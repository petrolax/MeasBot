#include <iostream>
#include <lua5.2/lua.hpp>
#include "SR570.hpp"

static int L_help(lua_State *L){
  static const std::string helpstring =
                    "Module for working with low noise current preamplifier SR570\n"
                    ;
  lua_pushstring(L, helpstring.c_str());
  return 1;
}

static int L_setSens(lua_State *L) {
  int sens;
  if(lua_gettop(L) >= 2) {
    if(lua_isnumber(L, 2)) sens = lua_tointeger(L, 2);
  } else {
    std::cerr << "SR570 error: This function must contain at least 1 parameter (port name)" << std::endl;
    return 0;
  }
  
  lua_getmetatable(L, 1);
  if(!lua_istable(L, -1)) {
    printf("Not metatable!\n");
    return 0;
  }

  lua_getfield(L, -1, "_objSR570_");
  if(!lua_islightuserdata(L, -1)) {
    printf("Not userdata!\n");
    return 0;
  }

  SR570 *sr = (SR570*)lua_touserdata(L, -1);
  if(sr == NULL) {
    fprintf(stderr, "Call 'connectNewDevice' before using anything functions");
    return 0;
  }
  sr->setSens(sens);
  
  return 0;
}

static int L_reset(lua_State *L) {
  lua_getmetatable(L, 1);
  if(!lua_istable(L, -1)) {
    printf("Not metatable!\n");
    return 0;
  }

  lua_getfield(L, -1, "_objSR570_");
  if(!lua_islightuserdata(L, -1)) {
    printf("Not userdata!\n");
    return 0;
  }

  SR570 *sr = (SR570*)lua_touserdata(L, -1);
  if(sr == NULL) {
    fprintf(stderr, "Call 'connectNewDevice' before using anything functions");
    return 0;
  }
  sr->reset();

  return 0;
}

static int L_setSenCal(lua_State *L) {
  int calMode;
  if(lua_gettop(L) >= 2) {
    if(lua_isnumber(L, 2)) calMode = lua_tointeger(L, 2);
  } else {
    std::cerr << "SR570 error: This function must contain at least 1 parameter (port name)" << std::endl;
    return 0;
  }

  lua_getmetatable(L, 1);
  if(!lua_istable(L, -1)) {
    printf("Not metatable!\n");
    return 0;
  }

  lua_getfield(L, -1, "_objSR570_");
  if(!lua_islightuserdata(L, -1)) {
    printf("Not userdata!\n");
    return 0;
  }

  SR570 *sr = (SR570*)lua_touserdata(L, -1);
  if(sr == NULL) {
    fprintf(stderr, "Call 'connectNewDevice' before using anything functions");
    return 0;
  }
  sr->setSenCal(calMode);

  return 0;
}

static int L_setSigInvertSense(lua_State *L) {
  int mode;
  if(lua_gettop(L) >= 2) {
    if(lua_isnumber(L, 2)) mode = lua_tointeger(L, 2);
  } else {
    std::cerr << "SR570 error: This function must contain at least 1 parameter (port name)" << std::endl;
    return 0;
  }

  lua_getmetatable(L, 1);
  if(!lua_istable(L, -1)) {
    printf("Not metatable!\n");
    return 0;
  }

  lua_getfield(L, -1, "_objSR570_");
  if(!lua_islightuserdata(L, -1)) {
    printf("Not userdata!\n");
    return 0;
  }

  SR570 *sr = (SR570*)lua_touserdata(L, -1);
  if(sr == NULL) {
    fprintf(stderr, "Call 'connectNewDevice' before using anything functions");
    return 0;
  }
  sr->setSigInvertSense(mode);

  return 0;
}

static int L_setUncalSensVernier(lua_State *L) {
  int scale;
  if(lua_gettop(L) >= 2) {
    if(lua_isnumber(L, 2)) scale = lua_tointeger(L, 2);
  } else {
    std::cerr << "SR570 error: This function must contain at least 1 parameter (port name)" << std::endl;
    return 0;
  }

  lua_getmetatable(L, 1);
  if(!lua_istable(L, -1)) {
    printf("Not metatable!\n");
    return 0;
  }

  lua_getfield(L, -1, "_objSR570_");
  if(!lua_islightuserdata(L, -1)) {
    printf("Not userdata!\n");
    return 0;
  }

  SR570 *sr = (SR570*)lua_touserdata(L, -1);
  if(sr == NULL) {
    fprintf(stderr, "Call 'connectNewDevice' before using anything functions");
    return 0;
  }
  sr->setUncalSensVernier(scale);

  return 0;
}

static int L_resetFilCap(lua_State *L) {
  lua_getmetatable(L, 1);
  if(!lua_istable(L, -1)) {
    printf("Not metatable!\n");
    return 0;
  }

  lua_getfield(L, -1, "_objSR570_");
  if(!lua_islightuserdata(L, -1)) {
    printf("Not userdata!\n");
    return 0;
  }

  SR570 *sr = (SR570*)lua_touserdata(L, -1);
  if(sr == NULL) {
    fprintf(stderr, "Call 'connectNewDevice' before using anything functions");
    return 0;
  }
  sr->resetFilCap();

  return 0;
}

static int L_disconnect(lua_State *L) {
  lua_getmetatable(L, 1);
  if(!lua_istable(L, -1)) {
    printf("Not metatable!\n");
    return 0;
  }

  lua_getfield(L, -1, "_objSR570_");
  if(!lua_islightuserdata(L, -1)) {
    printf("Not userdata!\n");
    return 0;
  }

  SR570 *sr = (SR570*)lua_touserdata(L, -1);
  if(sr == NULL) {
    fprintf(stderr, "Call 'connectNewDevice' before using anything functions");
    return 0;
  }
  sr->closePort();

  delete sr;
  lua_pushlightuserdata(L, nullptr);
  lua_setfield(L, 1, "_objSR570_");

  return 1;
}

static int L_connectNewDevice(lua_State *L) {
  char *portname = nullptr;
  int baud = 9600, checkError = 0;
  if(lua_gettop(L) >= 2) {
    if(lua_isstring(L, 2)) portname = (char*)lua_tostring(L, 2);
  } else {
    std::cerr << "SR570 error: This function must contain at least 1 parameter (port name)" << std::endl;
    return 0;
  }
  if(lua_gettop(L) >= 3) {
    if(lua_isnumber(L, 3)) baud = lua_tonumber(L, 3);
  }

  SR570 *sr;
  try {
    sr = new SR570;
  } catch (std::bad_alloc &ba) {
    std::cerr << ba.what() << std::endl;
    return 0;
  }

  checkError = sr->connect(portname, baud);

  if(checkError == 0) {
    std::cerr << "Port isn't open" << std::endl;
    delete sr;
    return 0;
  }

  lua_newtable(L);
    lua_newtable(L);
      lua_pushstring(L, "_objSR570_");
      lua_pushlightuserdata(L, sr);
      lua_rawset(L, -3);
      
      lua_pushstring(L, "_gcc");
      lua_pushcfunction(L, L_disconnect);
      lua_rawset(L, -3);
    lua_setmetatable(L, -2);

    lua_pushstring(L, "setSens");
    lua_pushcfunction(L, L_setSens);
    lua_rawset(L, -3);

    lua_pushstring(L, "reset");
    lua_pushcfunction(L, L_reset);
    lua_rawset(L, -3);

    lua_pushstring(L, "setSenCal");
    lua_pushcfunction(L, L_setSenCal);
    lua_rawset(L, -3);

    lua_pushstring(L, "setSigInvertSense");
    lua_pushcfunction(L, L_setSigInvertSense);
    lua_rawset(L, -3);

    lua_pushstring(L, "setUncalSensVernier");
    lua_pushcfunction(L, L_setUncalSensVernier);
    lua_rawset(L, -3);

    lua_pushstring(L, "resetFilCap");
    lua_pushcfunction(L, L_resetFilCap);
    lua_rawset(L, -3);

  return 1;
}

extern "C" int luaopen_sr570(lua_State *L) {
  lua_newtable(L);
    lua_pushcfunction(L, L_help);
    lua_setfield(L, -2, "help");

    lua_pushcfunction(L, L_connectNewDevice);
    lua_setfield(L, -2, "connectNewDevice");
  
  return 1;
}