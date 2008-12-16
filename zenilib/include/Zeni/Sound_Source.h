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

/**
 * \class Zeni::Sound_Source
 *
 * \ingroup Zenilib
 *
 * \brief Plays Sound Data
 *
 * A Sound_Source plays sound data from a Sound_Buffer.  The term comes
 * from the idea that OpenAL is built for 3D positional sound.  A Sound_Source
 * is defined by the Sound_Buffer it is playing as well as its position, 
 * velocity, and several other factors.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifdef ZENI_INLINES
#include <Zeni/Sound.hxx>
#endif

#ifndef ZENI_SOUND_SOURCE_H
#define ZENI_SOUND_SOURCE_H

#include <Zeni/Core.h>
#include <Zeni/Coordinate.h>
#include <Zeni/Sound.h>
#include <Zeni/Timer.h>
#include <Zeni/Vector3f.h>

#include <string>

#ifndef DISABLE_AL
#include <AL/alut.h>
#else

#define ALint int
#define ALuint unsigned int
#define ALfloat float
#define ALenum int

#define AL_NONE 0
#define AL_TRUE true
#define AL_FALSE false

#endif

namespace Zeni {

  const Sound_Buffer & get_Hello_World_Buffer();

  class Sound_Source_HW {
    Sound_Source_HW(const Sound_Source_HW &rhs);
    Sound_Source_HW & operator=(const Sound_Source_HW &rhs);

  public:
    Sound_Source_HW();
    Sound_Source_HW(const Sound_Buffer &buffer,
                 const float &pitch = 1.0f,
                 const float &gain = 1.0f,
                 const Point3f &position = Point3f(),
                 const Vector3f &velocity = Vector3f(),
                 const bool &looping = false);
    Sound_Source_HW(const ALuint &buffer,
                 const float &pitch = 1.0f,
                 const float &gain = 1.0f,
                 const Point3f &position = Point3f(),
                 const Vector3f &velocity = Vector3f(),
                 const bool &looping = false);
    ~Sound_Source_HW();

    inline void set_buffer(const Sound_Buffer &buffer); ///< Set the Sound_Buffer to be played.
    inline void set_pitch(const float &pitch = 1.0f); ///< Set the pitch.
    inline void set_gain(const float &gain = 1.0f); ///< Set the gain.
    inline void set_position(const Point3f &position); ///< Set the position of the Sound_Source_HW.
    inline void set_velocity(const Vector3f &velocity); ///< Set the velocity of the Sound_Source_HW for the doppler effect.
    inline void set_looping(const bool &looping); ///< Set whether the Sound_Buffer should loop back to the start once it is done playing.
    inline void set_time(const float &time); ///< Set the current position in the Sound_Buffer, offset in seconds.
    inline void set_near_clamp(const float &near_clamp = 10.0f); // Set the near clamping distance
    inline void set_far_clamp(const float &far_clamp = 1000.0f); // Set the far clamping distance
    inline void set_rolloff(const float &rolloff = 1.0f); // Set the maximum reduction in volume due to distance

    inline const Sound_Buffer & get_buffer() const; ///< Get the Sound_Buffer
    inline float get_pitch() const; ///< Get the pitch.
    inline float get_gain() const; ///< Get the gain.
    inline Point3f get_position() const; ///< Get the position of the Sound_Buffer.
    inline Vector3f get_velocity() const; ///< Get the velocity of the Sound_Buffer.
    inline bool is_looping() const; ///< Check to see if the Sound_Buffer is set to loop back to the start once it is done playing.
    inline float get_time() const; ///< Get the current position in the Sound_Buffer, offset in seconds.
    inline float get_near_clamp() const; // Get the near clamping distance
    inline float get_far_clamp() const; // Get the far clamping distance
    inline float get_rolloff() const; // Get the maximum reduction in volume due to distance

    inline void play(); ///< Begin playing or unpause the Sound_Source_HW.
    inline void pause(); ///< Pause the Sound_Source_HW.
    inline void stop(); ///< Stop the Sound_Source_HW.  (Essentially the same as pause but resets the current time.)

    inline bool is_playing(); ///< Check to see if the Sound_Source_HW is playing.
    inline bool is_paused(); ///< Check to see if the Sound_Source_HW is paused.
    inline bool is_stopped(); ///< Check to see if the Sound_Source_HW is stopped.

  private:
    void init(const Sound_Buffer &buffer,
              const float &pitch = 1.0f,
              const float &gain = 1.0f,
              const Point3f &position = Point3f(),
              const Vector3f &velocity = Vector3f(),
              const bool &looping = false);

    mutable ALuint m_source;
    mutable const Sound_Buffer * m_buffer;
  };

  class Sound_Source {
    Sound_Source(const Sound_Source &rhs);
    Sound_Source & operator=(const Sound_Source &rhs);

  public:
    Sound_Source();
    Sound_Source(const Sound_Buffer &buffer,
                 const float &pitch = 1.0f,
                 const float &gain = 1.0f,
                 const Point3f &position = Point3f(),
                 const Vector3f &velocity = Vector3f(),
                 const bool &looping = false);
    ~Sound_Source();

    inline void set_buffer(const Sound_Buffer &buffer); ///< Set the Sound_Buffer to be played.
    inline void set_pitch(const float &pitch = 1.0f); ///< Set the pitch.
    inline void set_gain(const float &gain = 1.0f); ///< Set the gain.
    inline void set_position(const Point3f &position); ///< Set the position of the Sound_Source.
    inline void set_velocity(const Vector3f &velocity); ///< Set the velocity of the Sound_Source for the doppler effect.
    inline void set_looping(const bool &looping); ///< Set whether the Sound_Buffer should loop back to the start once it is done playing.
    inline void set_time(const float &time); ///< Set the current position in the Sound_Buffer, offset in seconds.
    inline void set_near_clamp(const float &near_clamp = 10.0f); // Set the near clamping distance
    inline void set_far_clamp(const float &far_clamp = 1000.0f); // Set the far clamping distance
    inline void set_rolloff(const float &rolloff = 1.0f); // Set the maximum reduction in volume due to distance

    inline const Sound_Buffer & get_buffer() const; ///< Get the Sound_Buffer's OpenAL id
    inline float get_pitch() const; ///< Get the pitch.
    inline float get_gain() const; ///< Get the gain.
    inline Point3f get_position() const; ///< Get the position of the Sound_Buffer.
    inline Vector3f get_velocity() const; ///< Get the velocity of the Sound_Buffer.
    inline bool is_looping() const; ///< Check to see if the Sound_Buffer is set to loop back to the start once it is done playing.
    inline float get_time() const; ///< Get the current position in the Sound_Buffer, offset in seconds.
    inline float get_near_clamp() const; // Get the near clamping distance
    inline float get_far_clamp() const; // Get the far clamping distance
    inline float get_rolloff() const; // Get the maximum reduction in volume due to distance

    inline void play(); ///< Begin playing or unpause the Sound_Source.
    inline void pause(); ///< Pause the Sound_Source.
    inline void stop(); ///< Stop the Sound_Source.  (Essentially the same as pause but resets the current time.)

    inline bool is_playing(); ///< Check to see if the Sound_Source is playing.
    inline bool is_paused(); ///< Check to see if the Sound_Source is paused.
    inline bool is_stopped(); ///< Check to see if the Sound_Source is stopped.

  private:
    void init();

    Sound_Source_HW * m_hw;
    Time m_play_time;

    const Sound_Buffer * m_buffer;
    float m_pitch;
    float m_gain;
    Point3f m_position;
    Vector3f m_velocity;
    bool m_looping;
    float m_time;
    float m_near_clamp;
    float m_far_clamp;
    float m_rolloff;
  };

  struct Sound_Source_HW_Init_Failure : public Error {
    Sound_Source_HW_Init_Failure() : Error("Zeni Sound Source Hardware Failed to Initialize Correctly") {}
  };

}

#endif