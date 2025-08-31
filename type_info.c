
typedef enum Type_kind {
  TYPE_KIND_NULL = 0,
  TYPE_KIND_VOID, TYPE_KIND_FIRST_LEAF = TYPE_KIND_VOID,
  
  TYPE_KIND_U8,
  TYPE_KIND_U16,
  TYPE_KIND_U32,
  TYPE_KIND_U64,
  TYPE_KIND_S8,
  TYPE_KIND_S16,
  TYPE_KIND_S32,
  TYPE_KIND_S64,
  TYPE_KIND_B8,
  TYPE_KIND_B16,
  TYPE_KIND_B32,
  TYPE_KIND_B64,
  TYPE_KIND_F32,
  TYPE_KIND_F64, TYPE_KIND_LAST_LEAF = TYPE_KIND_F64,
  
  TYPE_KIND_PTR,
  TYPE_KIND_ARRAY,
  
  TYPE_KIND_STRUCT,
  TYPE_KIND_UNION,
  TYPE_KIND_ENUM,
  
  TYPE_KIND_COUNT

} Type_kind;

typedef u32 Type_flags;
//enum {
//};

typedef u32 Type_member_flags;
enum {
  TYPE_MEMBER_FLAG_NO_SERIALIZE = (1<<0),
};

typedef struct Type_member Type_member;
typedef struct Type_info Type_info;

struct Type_member {
  Str8 name;
  Type_info *type;
  u64 offset;
  Type_member_flags flags;
};

struct Type_info {
  Type_kind kind;
  Type_flags flags;
  u64 bytes;
  Type_info *child;
  Str8 count_delimiter_name;
  Str8 name;
  u64 member_or_array_count;
  Type_member *members;
};

typedef struct Type_serialize_ptr_ref_info Type_serialize_ptr_ref_info;
struct Type_serialize_ptr_ref_info {
  Type_info *type;      // pointers to this
  void *indexify_base;  // can be indexified using this
  void *offsetify_base; // can be offsetified using this
  void *nil_ptr;        // is terminal if matching 0 or this
};

typedef struct Type_serialize_params Type_serialize_params;
struct Type_serialize_params {
  u64 *deserialize_final_read_offset;
  Type_serialize_ptr_ref_info *ptr_ref_infos;
  u64 ptr_ref_infos_count;
};


read_only global Type_info type_nil = { .kind = TYPE_KIND_NULL, .child = &type_nil };
read_only global Type_member type_member_nil = { .type = &type_nil };


Type_member *type_member_from_name(Type_info *type, Str8 name);

void typed_data_rebase_ptrs(Type_info *type, Str8 data, void *base_ptr);
Str8 serialized_from_typed_data(Arena *arena, Type_info *type, Str8 data, Type_serialize_params *params);
Str8 deserialized_from_typed_data(Arena *arena, Type_info *type, Str8 data, Type_serialize_params *params);
Str8 deep_copy_from_typed_data(Arena *arena, Type_info *type, Str8 data, Type_serialize_params *params);


func Type_member *type_member_from_name(Type_info *type, Str8 name) {
  Type_member *member = &type_member_nil;

  if(type->members != 0 & name.len != 0) {
    for(u64 idx = 0; idx < type->member_or_array_count; idx += 1) {
      if(str8_match(type->members[idx].name, name)) {
        member = &type->members[idx];
        break;
      }
    }
  }

  return member;
}

func void typed_data_rebase_ptrs(Type_info *type, Str8 data, void *base_ptr) {
  Arena_scope scratch = scratch_begin(0, 0);

  typedef struct Rebase_type_task Rebase_type_task;
  struct Rebase_type_task {
    Rebase_type_task *next;
    Type_info *type;
    u8 *ptr;
  };

  Rebase_type_task start_task = { .next = 0, .type = type, .ptr = data.s };
  Rebase_type_task *first_task = &start_task;
  Rebase_type_task *last_task = first_task;

  for(Rebase_type_task *task = first_task; task != 0; task = task->next) {
    switch(task->type->kind) {
      default:
        break;
      case TYPE_KIND_PTR:
        {
          *((u64*)(task->ptr)) = ((u64)(*(u8**)task->ptr - (u8*)base_ptr));
        } break;
      case TYPE_KIND_ARRAY:
        {
          for(u64 idx = 0; idx < task->type->member_or_array_count; idx += 1) {
            Rebase_type_task *new_task = push_struct(scratch.arena, Rebase_type_task);
            new_task->type = task->type->child;
            new_task->ptr = task->ptr + task->type->child->bytes;
            sll_queue_push(first_task, last_task, new_task);
          }
        } break;
      case TYPE_KIND_STRUCT:
        {
          for(u64 idx = 0; idx < task->type->member_or_array_count; idx += 1) {
            Type_member *member = &(task->type->members[idx]);
            Rebase_type_task *new_task = push_struct(scratch.arena, Rebase_type_task);
            new_task->type = member->type;
            new_task->ptr = task->ptr + member->offset;
            sll_queue_push(first_task, last_task, new_task);
          }
        } break;
    }
  }

  scratch_end(scratch);
}

func Str8 serialized_from_typed_data(Arena *arena, Type_info *type, Str8 data, Type_serialize_params *params) {
  Arena_scope scratch = scratch_begin(&arena, 1);

  Str8_list strings = {0};

  str8_serial_begin(scratch.arena, &strings);
  {
    typedef struct Serialize_type_task Serialize_type_task;
    struct Serialize_type_task {
      Serialize_type_task *next;
      Type_info *type;
      u64 count;
      u8 *src;
      Type_info *containing_type;
      u8 *containing_ptr;
      b32 is_post_header;
    };

    Serialize_type_task start_task = { .next = 0, .type = type, .count = 1, .src = data.s };
    Serialize_type_task *first_task = &start_task;
    Serialize_type_task *last_task = first_task;

    for(Serialize_type_task *task = first_task; task != 0; task = task->next) {
      switch(task->type->kind) {
        default:
          if(TYPE_KIND_FIRST_LEAF <= task->type->kind && task->type->kind <= TYPE_KIND_LAST_LEAF) {
            Str8 str =
            {
              .s = task->src,
              .len = task->type->bytes * task->count,
            };
            str8_serial_push_str(scratch.arena, &strings, str);
          }
          break;
        case TYPE_KIND_PTR:
          {
            Type_serialize_ptr_ref_info *ptr_ref_info = 0;

            /* rjf: unpack info about this pointer */
            for(u64 idx = 0; idx < params->ptr_ref_infos_count; idx += 1) {
              if(params->ptr_ref_infos[idx].type == task->type->child) {
                ptr_ref_info = &params->ptr_ref_infos[idx];
                break;
              }
            }

            if(ptr_ref_info != 0 && ptr_ref_info->indexify_base != 0) {
              /* rjf: indexification -> subtract base, divide child type size, write index */

              u64 ptr_value = 0;
              memory_copy(&ptr_value, task->src, sizeof(ptr_value));
              u64 ptr_write_value =
                ( (u64)( (u8*)ptr_value - (u8*)ptr_ref_info->indexify_base ) / task->type->child->bytes );
              str8_serial_push_struct(scratch.arena, &strings, &ptr_write_value);

            } else if(ptr_ref_info != 0 && ptr_ref_info->offsetify_base != 0) {
              /* rjf: offsetification -> subtract base, write offsets */

              u64 ptr_value = 0;
              memory_copy(&ptr_value, task->src, sizeof(ptr_value));
              u64 ptr_write_value = (u64)( (u8*)ptr_value - (u8*)ptr_ref_info->offsetify_base );
              str8_serial_push_struct(scratch.arena, &strings, &ptr_write_value);

            } else if(task->type->count_delimiter_name.len != 0 && !task->is_post_header) {
              /*
               * rjf: size-by-member (pre-header): still potentially dependent on other members which
               * delimit our size, so push a new post-header task for pointer.
               *
               * jfd: basically we push the same task again and say that the count delimiter comes before us,
               * and on our next iteration in this loop we will run in to the count delimiter.
               */

              Serialize_type_task *new_task = push_struct(scratch.arena, Serialize_type_task);
              new_task->type = task->type;
              new_task->count = task->count;
              new_task->src = task->src;
              new_task->containing_type = task->containing_type;
              new_task->containing_ptr = task->containing_ptr;
              new_task->is_post_header = 1;
              sll_queue_push(first_task, last_task, new_task);

            } else if(task->type->count_delimiter_name.len != 0 && task->is_post_header) {

              /* 
               * rjf: size-by-member (post-header): all flat parts of containing struct have been
               * iterated, so now we can read the size, & descend to new task to read pointer
               * destination contents
               */

              u64 count = 0;
              {
                Type_member *count_member = type_member_from_name(task->containing_type, task->type->count_delimiter_name);
                memory_copy(&count, task->containing_ptr + count_member->offset, count_member->type->bytes);
              }

              Serialize_type_task *new_task = push_struct(scratch.arena, Serialize_type_task);
              new_task->type = task->type->child;
              new_task->count = count;
              new_task->src = *(void**)task->src;
              new_task->containing_type = task->containing_type;
              new_task->containing_ptr = task->containing_ptr;
              sll_queue_push(first_task, last_task, new_task);

            } else {
              Str8 str =
              {
                .s = task->src,
                .len = task->type->bytes * task->count,
              };
              str8_serial_push_str(scratch.arena, &strings, str);
            }

          } break;
        case TYPE_KIND_ARRAY:
          {
            Serialize_type_task *new_task = push_struct(scratch.arena, Serialize_type_task);
            new_task->type = task->type->child;
            new_task->count = task->type->member_or_array_count;
            new_task->src = task->src;
            new_task->containing_type = task->containing_type;
            new_task->containing_ptr = task->containing_ptr;
            sll_queue_push(first_task, last_task, new_task);
          } break;
        case TYPE_KIND_STRUCT:
          {
            for(u64 idx = 0; idx < task->count; idx += 1) {
              for(u64 member_idx = 0; member_idx < task->type->member_or_array_count; member_idx += 1) {
                if(task->type->members[member_idx].flags & TYPE_MEMBER_FLAG_NO_SERIALIZE) {
                  continue;
                }

                Type_member *member = &task->type->members[member_idx];

                Serialize_type_task *new_task = push_struct(scratch.arena, Serialize_type_task);
                new_task->type = member->type;
                new_task->count = 1;
                new_task->src = task->src + idx * task->type->bytes + member->offset;
                new_task->containing_type = task->type;
                new_task->containing_ptr = task->src;
                sll_queue_push(first_task, last_task, new_task);

              }
            }
          } break;
        case TYPE_KIND_UNION:
          {
            UNIMPLEMENTED;
          } break;
        case TYPE_KIND_ENUM:
          {
            Serialize_type_task *new_task = push_struct(scratch.arena, Serialize_type_task);
            new_task->type = task->type->child;
            new_task->count = task->count;
            new_task->src = task->src;
            new_task->containing_type = task->containing_type;
            new_task->containing_ptr = task->containing_ptr;
            sll_queue_push(first_task, last_task, new_task);
          } break;
      }
    }

  }

  Str8 result = str8_serial_end(arena, &strings);
  scratch_end(scratch);

  return result;
}

func Str8 deserialized_from_typed_data(Arena *arena, Type_info *type, Str8 data, Type_serialize_params *params) {
  Str8 result;
  result.len = type->bytes;
  result.s = push_array(arena, u8, result.len);

  {
    Arena_scope scratch = scratch_begin(&arena, 1);

    typedef struct Deserialize_type_task Deserialize_type_task;
    struct Deserialize_type_task {
      Deserialize_type_task *next;
      Type_info *type;
      u64 count;
      u8 *dest;
      Type_info *containing_type;
      u8 *containing_ptr;
      b32 is_post_header;
    };

    u64 read_offset = 0;
    Deserialize_type_task start_task = { .type = type, .count = 1, .dest = result.s };
    Deserialize_type_task *first_task = &start_task;
    Deserialize_type_task *last_task = first_task;

    for(Deserialize_type_task *task = first_task; task != 0; task = task->next) {
      u8 *task_src = data.s + read_offset;

      switch(task->type->kind) {
        default:
          if(TYPE_KIND_FIRST_LEAF <= task->type->kind && task->type->kind <= TYPE_KIND_LAST_LEAF) {
            memory_copy(task->dest, task_src, task->type->bytes * task->count);
            read_offset += task->type->bytes * task->count;
          }
          break;
        case TYPE_KIND_PTR:
          {
            Type_serialize_ptr_ref_info *ptr_ref_info = 0;

            /* rjf: unpack info about this pointer */
            for(u64 idx = 0; idx < params->ptr_ref_infos_count; idx += 1) {
              if(params->ptr_ref_infos[idx].type == task->type->child) {
                ptr_ref_info = &params->ptr_ref_infos[idx];
                break;
              }
            }

            if(ptr_ref_info != 0 && ptr_ref_info->indexify_base != 0) {
              /* rjf: indexification -> add base, multiply child type size */

              u64 ptr_value = 0;
              memory_copy(&ptr_value, task_src, sizeof(ptr_value));
              u64 ptr_write_value =
                (ptr_value + (u64)ptr_ref_info->indexify_base) * task->type->child->bytes;
              memory_copy(task->dest, &ptr_write_value, sizeof(ptr_write_value));
              read_offset += sizeof(ptr_value);

            } else if(ptr_ref_info != 0 && ptr_ref_info->offsetify_base != 0) {
              /* rjf: offsetification -> add base */

              u64 ptr_value = 0;
              memory_copy(&ptr_value, task_src, sizeof(ptr_value));
              u64 ptr_write_value = ptr_value + (u64)ptr_ref_info->offsetify_base;
              memory_copy(task->dest, &ptr_write_value, sizeof(ptr_write_value));
              read_offset += sizeof(ptr_value);

            } else if(task->type->count_delimiter_name.len != 0 && !task->is_post_header) {
              /*
               * rjf: size-by-member (pre-header): still potentially dependent on other members which
               * delimit our size, so push a new post-header task for pointer.
               *
               * jfd: basically we push the same task again and say that the count delimiter comes before us,
               * and on our next iteration in this loop we will run in to the count delimiter.
               */

              Deserialize_type_task *new_task = push_struct(scratch.arena, Deserialize_type_task);
              new_task->type = task->type;
              new_task->count = task->count;
              new_task->dest = task->dest;
              new_task->containing_type = task->containing_type;
              new_task->containing_ptr = task->containing_ptr;
              new_task->is_post_header = 1;
              sll_queue_push(first_task, last_task, new_task);

            } else if(task->type->count_delimiter_name.len != 0 && task->is_post_header) {

              /* 
               * rjf: size-by-member (post-header): all flat parts of containing struct have been
               * iterated, so now we can read the size, & descend to new task to read pointer
               * destination contents
               */

              u64 count = 0;
              {
                Type_member *count_member = type_member_from_name(task->containing_type, task->type->count_delimiter_name);
                memory_copy(&count, task->containing_ptr + count_member->offset, count_member->type->bytes);
              }

              u64 ptr_dest_buffer_size = (count+1)*task->type->child->bytes;
              u8 *ptr_dest_buffer = push_array(arena, u8, ptr_dest_buffer_size);
              memory_copy(task->dest, &ptr_dest_buffer, sizeof(ptr_dest_buffer));

              Deserialize_type_task *new_task = push_struct(scratch.arena, Deserialize_type_task);
              new_task->type = task->type->child;
              new_task->count = count;
              new_task->dest = ptr_dest_buffer;
              new_task->containing_type = task->containing_type;
              new_task->containing_ptr = task->containing_ptr;
              sll_queue_push(first_task, last_task, new_task);

            } else {
              memory_copy(task->dest, task_src, task->type->bytes * task->count);
              read_offset += task->type->bytes * task->count;
            }

          } break;
        case TYPE_KIND_ARRAY:
          {
            Deserialize_type_task *new_task = push_struct(scratch.arena, Deserialize_type_task);
            new_task->type = task->type->child;
            new_task->count = task->type->member_or_array_count;
            new_task->dest = task->dest;
            new_task->containing_type = task->containing_type;
            new_task->containing_ptr = task->containing_ptr;
            sll_queue_push(first_task, last_task, new_task);
          } break;
        case TYPE_KIND_STRUCT:
          {
            for(u64 idx = 0; idx < task->count; idx += 1) {
              for(u64 member_idx = 0; member_idx < task->type->member_or_array_count; member_idx += 1) {
                if(task->type->members[member_idx].flags & TYPE_MEMBER_FLAG_NO_SERIALIZE) {
                  continue;
                }

                Type_member *member = &task->type->members[member_idx];

                Deserialize_type_task *new_task = push_struct(scratch.arena, Deserialize_type_task);
                new_task->type = member->type;
                new_task->count = 1;
                new_task->dest = task->dest + idx * task->type->bytes + member->offset;
                new_task->containing_type = task->type;
                new_task->containing_ptr = task->dest;
                sll_queue_push(first_task, last_task, new_task);
              }
            }
          } break;
        case TYPE_KIND_UNION:
          {
            UNIMPLEMENTED;
          } break;
        case TYPE_KIND_ENUM:
          {
            Deserialize_type_task *new_task = push_struct(scratch.arena, Deserialize_type_task);
            new_task->type = task->type->child;
            new_task->count = task->count;
            new_task->dest = task->dest;
            new_task->containing_type = task->containing_type;
            new_task->containing_ptr = task->containing_ptr;
            sll_queue_push(first_task, last_task, new_task);
          } break;
      }

    }

    if(params->deserialize_final_read_offset != 0) {
      params->deserialize_final_read_offset[0] = read_offset;
    }

    scratch_end(scratch);
  }

  return result;
}

func Str8 deep_copy_from_typed_data(Arena *arena, Type_info *type, Str8 data, Type_serialize_params *params) {
  Arena_scope scratch = scratch_begin(&arena, 1);
  Str8 data_serialized = serialized_from_typed_data(scratch.arena, type, data, params);
  Str8 data_copy = deserialized_from_typed_data(arena, type, data_serialized, params);
  scratch_end(scratch);
  return data_copy;
}

