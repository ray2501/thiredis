/*
 * For C++ compilers, use extern "C"
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <tcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hiredis.h"

static int Redis_Cmd(ClientData arg, Tcl_Interp * interp, int objc, Tcl_Obj * const objv[]);

/*
 * Only the _Init function is exported.
 */

extern DLLEXPORT int	Thiredis_Init(Tcl_Interp * interp);

/*
 * end block for C++
 */

#ifdef __cplusplus
}
#endif

redisContext *c = NULL;

static int Redis_Cmd(ClientData arg, Tcl_Interp * interp, int objc, Tcl_Obj * const objv[])
{
  int cmd;

  enum {
     CMD_CONNECT, CMD_CONNECTUNIX, CMD_DISCONNECT,
     CMD_COMMAND, CMD_APPENDCOMMAND, CMD_REPLY,
  };

  static const char *sCmd[] = {
    "connect", "connectUnix", "disconnect",
    "command", "appendcommand", "reply",
    0
  };

  if (objc < 2) {
    Tcl_WrongNumArgs(interp, 1, objv, "cmd args");
    return TCL_ERROR;
  }

  if (Tcl_GetIndexFromObj(interp, objv[1], sCmd, "command", TCL_EXACT, (int *) &cmd) != TCL_OK) {
    return TCL_ERROR;
  }

  switch (cmd) {
    case CMD_CONNECT: {
      Tcl_Obj *return_obj;
      struct timeval timeout = { 3, 0 }; // 3 seconds
      char *hostname = NULL;
      int port = 0;
      Tcl_Size len = 0;

      if( objc != 4){
        Tcl_WrongNumArgs(interp, 1, objv, "hostname port");
        return TCL_ERROR;
      }

      hostname = Tcl_GetStringFromObj(objv[2], &len);
      if( !hostname || len < 1 ) {
          return TCL_ERROR;
      }

      if(Tcl_GetIntFromObj(interp, objv[3], &port) != TCL_OK) {
          return TCL_ERROR;
      }

      if(port < 0 || port > 65535) {
          Tcl_AppendResult(interp, "port number must be in range [0..65535]", (char*)0);
          return TCL_ERROR;
      }

      c = redisConnectWithTimeout(hostname, port, timeout);
      if(c == NULL || c->err) {
         return_obj = Tcl_NewBooleanObj(0);
      } else {
         return_obj = Tcl_NewBooleanObj(1);
      }

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    case CMD_CONNECTUNIX: {
      Tcl_Obj *return_obj;
      struct timeval timeout = { 3, 0 }; // 3 seconds
      char *path = NULL;
      Tcl_Size len = 0;

      if( objc != 3){
        Tcl_WrongNumArgs(interp, 1, objv, "path");
        return TCL_ERROR;
      }

      path = Tcl_GetStringFromObj(objv[2], &len);
      if( !path || len < 1 ) {
        return TCL_ERROR;
      }

      c = redisConnectUnixWithTimeout(path, timeout);
      if(c == NULL || c->err) {
        return_obj = Tcl_NewBooleanObj(0);
      } else {
        return_obj = Tcl_NewBooleanObj(1);
      }

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    case CMD_DISCONNECT: {
      Tcl_Obj *return_obj;

      if( objc != 2){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      if(c == NULL)
        return TCL_ERROR;

      redisFree(c);
      c = NULL;
      return_obj = Tcl_NewBooleanObj(1);

      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    case CMD_COMMAND: {
      Tcl_Obj *return_obj;
      char *command = NULL;
      Tcl_Size len = 0;
      redisReply *reply;

      if( objc != 3){
        Tcl_WrongNumArgs(interp, 2, objv, "command");
        return TCL_ERROR;
      }

      command = Tcl_GetStringFromObj(objv[2], &len);
      if( !command || len < 1 ) {
          return TCL_ERROR;
      }

      if(c == NULL)
        return TCL_ERROR;

      reply = redisCommand(c, command);
      return_obj = Tcl_NewStringObj( reply->str, -1 );
      freeReplyObject(reply);

      Tcl_SetObjResult(interp, return_obj);

      break;
    }

    case CMD_APPENDCOMMAND: {
      Tcl_Obj *return_obj;
      char *command = NULL;
      Tcl_Size len = 0;

      if( objc != 3){
        Tcl_WrongNumArgs(interp, 2, objv, "command");
        return TCL_ERROR;
      }

      command = Tcl_GetStringFromObj(objv[2], &len);
      if( !command || len < 1 ) {
          return TCL_ERROR;
      }

      if(c == NULL)
        return TCL_ERROR;

      redisAppendCommand(c, command);

      return_obj = Tcl_NewBooleanObj(1);
      Tcl_SetObjResult(interp, return_obj);
      break;
    }

    case CMD_REPLY: {
      Tcl_Obj *return_obj;
      redisReply *reply;

      if( objc != 2){
        Tcl_WrongNumArgs(interp, 2, objv, 0);
        return TCL_ERROR;
      }

      if(c == NULL)
        return TCL_ERROR;

      redisGetReply(c, (void **) &reply);
      return_obj = Tcl_NewStringObj( reply->str, -1 );
      freeReplyObject(reply);

      Tcl_SetObjResult(interp, return_obj);

      break;
    }
  }

  return TCL_OK;
}


/*
 *----------------------------------------------------------------------
 *
 * Thiredis_Init --
 *
 *	Initialize the new package.
 *
 * Results:
 *	A standard Tcl result
 *
 * Side effects:
 *	The thiredis package is created.
 *
 *----------------------------------------------------------------------
 */

int
Thiredis_Init(Tcl_Interp *interp)
{
    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
	return TCL_ERROR;
    }

    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK) {
	return TCL_ERROR;
    }

    Tcl_CreateObjCommand(interp, "hiredis", Redis_Cmd, NULL, NULL);

    return TCL_OK;
}
