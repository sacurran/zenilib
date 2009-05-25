/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#ifndef ZENI_GAME_HXX
#define ZENI_GAME_HXX

// HXXed below
#include <Zeni/Gamestate.h>
#include <Zeni/Mutex.h>

#include <Zeni/Game.h>

namespace Zeni {

  Gamestate_Base & Game::get_current_state() {
    if(m_states.empty())
      throw Zero_Gamestate();
    
    return m_states.top().get();
  }

  size_t Game::size() const {
    return m_states.size();
  }

  void Game::push_state(const Gamestate &state) {
    m_states.push(state);
  }

  Gamestate Game::pop_state() {
    Gamestate gs;
    
    {
      if(m_states.empty())
        throw Zero_Gamestate();

      gs = m_states.top();
      m_states.pop();
    }

    return gs;
  }

  void Game::on_event(const SDL_Event &event) {
    Gamestate gs;
    
    {
      if(m_states.empty())
        throw Zero_Gamestate();

      gs = m_states.top();
    }
    
    gs.on_event(event);
  }

  void Game::perform_logic() {
    Gamestate gs;
    
    {
      if(m_states.empty())
        throw Zero_Gamestate();

      gs = m_states.top();
    }
    
    gs.perform_logic();
  }

  void Game::render() {
    {
      Gamestate gs;
      
      {
        if(m_states.empty())
          throw Zero_Gamestate();

        gs = m_states.top();
      }
      
      gs.render();
    }

    calculate_fps();
  }

  int Game::get_fps() const {
    return fps;
  }

}

#include <Zeni/Gamestate.hxx>
#include <Zeni/Mutex.hxx>

#endif
