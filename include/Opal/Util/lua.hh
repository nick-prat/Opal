#ifndef _LUA_H
#define _LUA_H

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <LuaBridge/LuaBridge.h>

// NOTE Helper class to make sure the lua state is closed last
class LuaState {
public:
    LuaState()
    : m_lState(luaL_newstate()) {
        luaL_openlibs(m_lState);
    }

    LuaState(const LuaState&) = delete;

    LuaState(LuaState &&luaState)
    : m_lState(luaState.m_lState) {
        luaState.m_lState = nullptr;
    }

    ~LuaState() {
        if(m_lState != nullptr) {
            lua_close(m_lState);
            m_lState = nullptr;
        }
    }

    LuaState &operator=(LuaState&) = delete;

    inline LuaState &operator=(LuaState &&luaState) {
        m_lState = luaState.m_lState;
        luaState.m_lState = nullptr;
        return *this;
    }

    inline lua_State* get() {
        return m_lState;
    }

private:
    lua_State* m_lState;
};

#endif // _LUA_H
