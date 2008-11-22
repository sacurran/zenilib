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

#include <Zeni/Shader.hxx>

#include <Zeni/Video.hxx>
#include <iostream>

using namespace std;

namespace Zeni {

  Shader_System::Shader_System()
    : m_context(cgCreateContext())
  {
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    get_Video().initialize(*this);
  }

  Shader_System::~Shader_System() {
    cgDestroyContext(m_context);
  }

  Shader_System & get_Shader_System() {
    static Shader_System e_shader_system;
    return e_shader_system;
  }
  
#ifndef DISABLE_GL
  void Shader_System::init(Video_GL &) {
    m_cg_vertex_profile = cgGLGetLatestProfile(CG_GL_VERTEX);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    m_cg_fragment_profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();
  }
#endif

#ifndef DISABLE_DX9
  void Shader_System::init(Video_DX9 &screen) {
    cgD3D9SetDevice(screen.get_d3d_device());

    m_cg_vertex_profile = cgD3D9GetLatestVertexProfile();
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();

    m_cg_fragment_profile = cgD3D9GetLatestPixelProfile();
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_System_Init_Failure();
  }
#endif
  
  Shader::Shader()
    : m_program(0)
  {
  }

  Shader::~Shader() {
    cgDestroyProgram(m_program);
  }

#ifndef DISABLE_GL
  void Shader::init(const std::string &filename, const std::string &entry_function, const CGprofile &profile, Video_GL &) {
    Shader_System &shader_system = get_Shader_System();
    const CGcontext &context = shader_system.get_context();

    cgGLSetOptimalOptions(profile);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Init_Failure();

    m_program =
      cgCreateProgramFromFile(
        context,                /* Cg runtime context */
        CG_SOURCE,              /* Program in human-readable form */
        filename.c_str(),       /* Name of file containing program */
        profile,                /* Profile: OpenGL ARB vertex program */
        entry_function.c_str(), /* Entry function name */
        NULL);                  /* No extra compiler options */
    if(cgGetError() != CG_NO_ERROR) {
      cerr << "Error initializing '" << filename.c_str() << "'\n";
      throw Shader_Init_Failure();
    }

    cgGLLoadProgram(m_program);
    if(cgGetError() != CG_NO_ERROR) {
      cerr << "Error initializing '" << filename.c_str() << "'\n";
      throw Shader_Init_Failure();
    }
  }

  void Shader::set(const CGprofile &profile, Video_GL &) const {
    cgGLBindProgram(m_program);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();

    cgGLEnableProfile(profile);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }

  void Shader::unset(const CGprofile &profile, Video_GL &) const {
    cgGLDisableProfile(profile);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Bind_Failure();
  }
#endif

#ifndef DISABLE_DX9
  void Shader::init(const std::string &filename, const std::string &entry_function, const CGprofile &profile, Video_DX9 &screen) {
    Shader_System &shader_system = get_Shader_System();
    const CGcontext &context = shader_system.get_context();

    const char **options = cgD3D9GetOptimalOptions(profile);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Init_Failure();

    m_program =
      cgCreateProgramFromFile(
        context,                /* Cg runtime context */
        CG_SOURCE,              /* Program in human-readable form */
        filename.c_str(),       /* Name of file containing program */
        profile,                /* Profile: OpenGL ARB vertex program */
        entry_function.c_str(), /* Entry function name */
        options);               /* No extra compiler options */
    if(cgGetError() != CG_NO_ERROR) {
      cerr << "Error initializing '" << filename.c_str() << "'\n";
      throw Shader_Init_Failure();
    }

    reload(screen);
  }

  void Shader::reload(Video_DX9 &) {
    cgD3D9LoadProgram(m_program, false, 0);
    if(cgGetError() != CG_NO_ERROR)
      throw Shader_Init_Failure();
  }

  void Shader::set(const CGprofile &, Video_DX9 &screen) const {
    try
    {
      cgD3D9BindProgram(m_program);
      if(cgGetError() != CG_NO_ERROR)
        throw Shader_Bind_Failure();
    }
    catch(Shader_Bind_Failure &bind_failure)
    {
      try
      {
        const_cast<Shader &>(*this).reload(screen);
      }
      catch(...)
      {
        throw bind_failure;
      }
    }
  }

  void Shader::unset(const CGprofile &, Video_DX9 &) const {
  }
#endif

}
