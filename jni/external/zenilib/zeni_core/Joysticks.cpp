/* This file is part of the Zenipex Library (zenilib).
 * Copyright (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * zenilib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * zenilib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with zenilib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <zeni_core.h>

#include <cassert>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <Zeni/Singleton.hxx>

namespace Zeni {

  template class Singleton<Joysticks>;

  Joysticks * Joysticks::create() {
    return new Joysticks;
  }

  Singleton<Joysticks>::Uninit Joysticks::g_uninit;
  Singleton<Joysticks>::Reinit Joysticks::g_reinit;

  Joysticks::Joysticks() {
    Core::remove_post_reinit(&g_reinit);

    /** Initialize SDL itself **/
    Core &cr = get_Core();

    /** Initialize Joysticks **/
    init();

    cr.lend_pre_uninit(&g_uninit);
    cr.lend_post_reinit(&g_reinit);
  }

  Joysticks::~Joysticks() {
    Core::remove_pre_uninit(&g_uninit);

    uninit();
  }

  Joysticks & get_Joysticks() {
    return Joysticks::get();
  }

  Sint32 Joysticks::get_joystick_id(const Sint32 &index) const {
    return SDL_JoystickInstanceID(m_joysticks.at(index)->joystick);
  }

  Sint32 Joysticks::get_joystick_index(const Sint32 &id) const {
    for(int i = 0, iend = int(m_joysticks.size()); i != iend; ++i) {
      if(SDL_JoystickInstanceID(m_joysticks[i]->joystick) == id)
        return i;
    }

    return -1;
  }

  size_t Joysticks::get_num_joysticks() const {
      return m_joysticks.size();
  }

  const char * Joysticks::get_joystick_name(const Sint32 &index) const {
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickName(m_joysticks[index]->joystick);
  }
  
  int Joysticks::get_joystick_num_axes(const Sint32 &index) const {
#ifdef ANDROID
    return 0;
#else
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickNumAxes(m_joysticks[index]->joystick);
#endif
  }

  int Joysticks::get_joystick_num_balls(const Sint32 &index) const {
#ifdef ANDROID
    return 0;
#else
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickNumBalls(m_joysticks[index]->joystick);
#endif
  }

  int Joysticks::get_joystick_num_hats(const Sint32 &index) const {
#ifdef ANDROID
    return 0;
#else
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickNumAxes(m_joysticks[index]->joystick);
#endif
  }

  int Joysticks::get_joystick_num_buttons(const Sint32 &index) const {
#ifdef ANDROID
    return 0;
#else
    assert(index >= 0 && Uint32(index) < m_joysticks.size());
    return SDL_JoystickNumAxes(m_joysticks[index]->joystick);
#endif
  }
  
  bool Joysticks::is_joystick_connected(const Sint32 &index) const {
    return SDL_JoystickGetAttached(m_joysticks[index]->joystick) != SDL_FALSE;
  }

  void Joysticks::reinit() {
    uninit();
    init();
  }

  void Joysticks::enable(const bool &
#ifndef ANDROID
    enable_
#endif
  ) {
#ifndef ANDROID
    SDL_JoystickEventState(enable_ ? SDL_ENABLE : SDL_DISABLE);
#endif
  }
  
  void Joysticks::poll() {
    for(int i = 0, iend = SDL_NumJoysticks(); i != iend; ++i) {
      SDL_Joystick * const joystick = SDL_JoystickOpen(i);
      if(!joystick) {
        uninit();
        throw Joystick_Init_Failure();
      }

      Joystick_Array::iterator found = m_joysticks.end();
      for(Joystick_Array::iterator it = m_joysticks.begin(), iend = m_joysticks.end(); it != iend; ++it) {
        if(SDL_JoystickInstanceID(joystick) == SDL_JoystickInstanceID((*it)->joystick)) {
          found = it;
          break;
        }
        else if(SDL_JoystickGetAttached((*it)->joystick) == SDL_FALSE)
          found = it;
      }
      
      if(found != m_joysticks.end() && SDL_JoystickInstanceID(joystick) == SDL_JoystickInstanceID((*found)->joystick))
        continue;

      Joystick_Info * info = new Joystick_Info();
      info->joystick = joystick;

      if(found == m_joysticks.end())
        m_joysticks.push_back(info);
      else {
        delete *found;
        *found = info;
      }

      char szGUID[33];
      SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), szGUID, sizeof(szGUID));
      info->gamecontroller = SDL_GameControllerOpen(i);
      if(info->gamecontroller) {
        char * mapping = SDL_GameControllerMapping(info->gamecontroller);
        std::cerr << "Loaded game controller: " << mapping << std::endl;
        SDL_free(mapping);
      }
      else {
        std::cerr << "Failed to load game controller (" << szGUID << "): " << SDL_GetError() << std::endl;
        SDL_ClearError();
      }

      info->haptic = SDL_JoystickIsHaptic(joystick) == 1 ? SDL_HapticOpenFromJoystick(joystick) : nullptr;
      if(info->haptic) {
        if(SDL_HapticEffectSupported(info->haptic, &info->haptic_effect) == SDL_TRUE) {
          info->haptic_effect_id = SDL_HapticNewEffect(info->haptic, &info->haptic_effect);
          if(SDL_HapticRunEffect(info->haptic, info->haptic_effect_id, 1) == -1)
            std::cerr << "Failed to run effect on joystick (" << szGUID << ")." << std::endl;
        }
      }
      else {
        std::cerr << "Joystick (" << szGUID << ") not recognized as haptic." << std::endl;
        SDL_ClearError();
      }
    }
    
  }

  void Joysticks::set_vibration(const size_t &index, const float &left, const float &right) {
    if(index < m_joysticks.size() && m_joysticks[index]->haptic_effect_id > -1) {
      m_joysticks[index]->haptic_effect.leftright.large_magnitude = Uint16(left * 0xFFFF);
      m_joysticks[index]->haptic_effect.leftright.small_magnitude = Uint16(right * 0xFFFF);
      if(m_joysticks[index]->haptic_effect_id > -1) {
        //switch(SDL_HapticGetEffectStatus(m_joysticks[index]->haptic, m_joysticks[index]->haptic_effect_id)) {
        //  case 0:
            //SDL_HapticRunEffect(m_joysticks[index]->haptic, m_joysticks[index]->haptic_effect_id, 1);
        //    break;
        //  case 1: 
            SDL_HapticUpdateEffect(m_joysticks[index]->haptic, m_joysticks[index]->haptic_effect_id, &m_joysticks[index]->haptic_effect);
        //    break;
        //  default:
        //    break;
        //}
      }
    }
  }

  Joysticks::Joystick_Info::Joystick_Info()
    : joystick(nullptr),
    gamecontroller(nullptr),
    haptic(nullptr),
    haptic_effect_id(-1)
  {
    memset(&haptic_effect, 0, sizeof(haptic_effect));
    haptic_effect.type = SDL_HAPTIC_LEFTRIGHT;
    haptic_effect.leftright.length = SDL_HAPTIC_INFINITY;
  }

  Joysticks::Joystick_Info::~Joystick_Info() {
    get_Core().assert_no_error();
    if(haptic)
      SDL_HapticClose(haptic);
    if(gamecontroller)
      SDL_GameControllerClose(gamecontroller);
    SDL_JoystickClose(joystick);
    get_Core().assert_no_error();
  }
  
  void Joysticks::init() {
#ifndef ANDROID
    if(SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) == -1)
      throw Joystick_Init_Failure();

    const String appdata_path = get_File_Ops().get_appdata_path();
    const String user_normal = appdata_path + "config/zenilib.xml";
    const String local_normal = "config/zenilib.xml";
    XML_Document file;

    if(file.try_load(local_normal)) {
      XML_Element_c zenilib = file["Zenilib"];
      XML_Element_c joysticks = zenilib["Joysticks"];
      if(joysticks.good()) {
        for(XML_Element_c joystick = joysticks.first(); joystick.good(); joystick = joystick.next()) {
          if(joystick.value() == "GameController") {
            if(SDL_GameControllerAddMapping(joystick.to_string().c_str()) < 0)
              std::cerr << "Joystick mapping " << joystick.to_string().c_str() << " failed." << std::endl;
          }
        }
      }
    }
    
    if(file.try_load(user_normal)) {
      XML_Element_c zenilib = file["Zenilib"];
      XML_Element_c joysticks = zenilib["Joysticks"];
      if(joysticks.good()) {
        for(XML_Element_c joystick = joysticks.first(); joystick.good(); joystick = joystick.next()) {
          if(joystick.value() == "GameController") {
            if(SDL_GameControllerAddMapping(joystick.to_string().c_str()) < 0)
              std::cerr << "Joystick mapping " << joystick.to_string().c_str() << " failed." << std::endl;
          }
        }
      }
    }

    SDL_JoystickEventState(SDL_ENABLE);
#endif
  }

  void Joysticks::uninit() {
#ifndef ANDROID
    SDL_JoystickEventState(SDL_DISABLE);

    for(Joystick_Array::iterator it = m_joysticks.begin(), iend = m_joysticks.end(); it != iend; ++it)
      delete *it;
    m_joysticks.clear();

    SDL_QuitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);

    get_Core().assert_no_error();
#endif
  }

#ifdef ENABLE_XINPUT
  Joysticks::XInputEnable_fcn Joysticks::g_XInputEnable = 0;
  Joysticks::XInputGetCapabilities_fcn Joysticks::g_XInputGetCapabilities = 0;
  Joysticks::XInputGetState_fcn Joysticks::g_XInputGetState = 0;
  Joysticks::XInputSetState_fcn Joysticks::g_XInputSetState = 0;
#endif

}
