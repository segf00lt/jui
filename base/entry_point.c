
#error entry_point module unimplemented

func void main_thread_base_entry_point(int arguments_count, char **arguments) {
  Arena_scope scratch = scratch_begin(0, 0);
  //ThreadNameF("[main thread]");
  
  //- rjf: set up telemetry
#if 0
#if PROFILE_TELEMETRY
  local_persist char tm_data[MB(64)];
  tmLoadLibrary(TM_RELEASE);
  tmSetMaxThreadCount(256);
  tmInitialize(sizeof(tm_data), tm_data);
#endif
  
  //- rjf: set up spall
#if PROFILE_SPALL
  spall_profile = spall_init_file_ex("spall_capture", 1, 0);
#endif
  
  //- rjf: parse command line
  String8List command_line_argument_strings = os_string_list_from_argcv(scratch.arena, arguments_count, arguments);
  CmdLine cmdline = cmd_line_from_string_list(scratch.arena, command_line_argument_strings);
  
  //- rjf: begin captures
  B32 capture = cmd_line_has_flag(&cmdline, str8_lit("capture"));
  if(capture)
  {
    ProfBeginCapture(arguments[0]);
  }
  
#if PROFILE_TELEMETRY 
  tmMessage(0, TMMF_ICON_NOTE, BUILD_TITLE);
#endif
#endif
  
  //- rjf: initialize all included layers
  
  //- rjf: call into entry point
  entry_point(arguments_count, arguments);
  
#if 0
  //- rjf: end captures
  if(capture) {
    ProfEndCapture();
  }
#endif
  
  scratch_end(scratch);
}

func void supplement_thread_base_entry_point(void (*entry_point)(void *params), void *params) {
  Tctx *tctx = tctx_alloc();
  tctx_select(tctx);
  entry_point(params);
  tctx_free(tctx);
}
