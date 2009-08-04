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

#ifndef ZENI_VIDEO_HXX
#define ZENI_VIDEO_HXX

// HXXed below
#include <Zeni/Camera.h>
#include <Zeni/Renderable.h>
#include <Zeni/Textures.h>
#include <Zeni/Quaternion.h>
#include <Zeni/Video_DX9.h>
#include <Zeni/Video_GL.h>

#include <Zeni/Video.h>

#include <Zeni/Global.h>

namespace Zeni {

  const Video_Base::VIDEO_MODE & Video::get_video_mode() {
    return g_video_mode;
  }

  const bool & Video::is_enabled() {
    return g_enabled;
  }

  const Point2i & Video::get_screen_size() {
    return g_screen_size;
  }

  const int & Video::get_screen_width() {
    return g_screen_size.x;
  }

  const int & Video::get_screen_height() {
    return g_screen_size.y;
  }

  const bool & Video::is_fullscreen() {
    return g_screen_full;
  }

  const bool & Video::is_frame_visible() {
    return g_screen_show_frame;
  }

  const bool & Video::get_backface_culling() {
    return g_backface_culling;
  }

  const bool & Video::get_lighting() {
    return g_lighting;
  }

  const bool & Video::get_normal_interpolation() {
    return g_normal_interp;
  }

  const bool & Video::get_vertical_sync() {
    return g_vertical_sync;
  }

  const int & Video::get_multisampling() {
    return g_multisampling;
  }

  const bool & Video::is_zwrite_enabled() const {
    return m_zwrite;
  }

  const bool & Video::is_ztest_enabled() const {
    return m_ztest;
  }

  const bool & Video::is_alpha_test_enabled() const {
    return m_alpha_test;
  }

  const Video::TEST & Video::get_alpha_test_function() const {
    return m_alpha_function;
  }

  const float & Video::get_alpha_test_value() const {
    return m_alpha_value;
  }

  void Video::set_2d() {
    set_2d_view(std::make_pair(Point2f(0.0f, 0.0f), Point2f(float(get_screen_width()), float(get_screen_height()))),
                std::make_pair(Point2i(0, 0), Point2i(get_screen_width(), get_screen_height())));
  }

  void Video::set_2d(const std::pair<Point2f, Point2f> &camera2d) {
    set_2d_view(camera2d,
                std::make_pair(Point2i(0, 0), Point2i(get_screen_width(), get_screen_height())));
  }

  void Video::set_3d(const Camera &camera) {
    set_3d_view(camera,
                std::make_pair(Point2i(0, 0), Point2i(get_screen_width(), get_screen_height())));
  }

  const Color & Video::get_color() const {
    return m_color;
  }

  const Color & Video::get_clear_color() const {
    return m_clear_color;
  }

  void Video::apply_texture(const std::string &name) {
    apply_texture(get_Textures().get_id(name));
  }

  const bool & Video::is_initialized() {
    return g_initialized;
  }

  SDL_Surface * Video::get_display_surface() {
    return m_display_surface;
  }

  const bool & Video::get_opengl_flag() const {
    return m_opengl_flag;
  }

  void Video::set_opengl_flag(const bool &on) {
    m_opengl_flag = on;
  }

  void Video::rotate_scene(const Quaternion &rotation) {
    const std::pair<Vector3f, float> rayngel = rotation.get_rotation();
    rotate_scene(rayngel.first, rayngel.second);
  }

  const Matrix4f & Video::get_view_matrix() const {
    return m_view;
  }

  const Matrix4f & Video::get_projection_matrix() const {
    return m_projection;
  }

  const std::pair<Point2i, Point2i> & Video::get_viewport() const {
    return m_viewport;
  }

  const std::string & Video::get_title() const {
    return m_title;
  }

  const std::string & Video::get_taskmsg() const {
    return m_taskmsg;
  }

  /* Note that variadic macros *may* not be supported in some pre-C99 compilers.
   * It is not hard to code around, but they are convenient.
   */

#ifndef DISABLE_DX9
#ifndef DISABLE_GL

#define \
  VIDEO_IV_FCN_CALL(member_function, ...) { \
    switch(vtype()) { \
      case Video_Base::ZENI_VIDEO_GL: return reinterpret_cast<Video_GL *>(this)->member_function(__VA_ARGS__); \
      case Video_Base::ZENI_VIDEO_DX9: return reinterpret_cast<Video_DX9 *>(this)->member_function(__VA_ARGS__); \
      default: abort(); /* implies ZENI_VIDEO_ANY, which *should* be impossible */ \
    } \
    exit(END_OF_TIME); /* cannot be called, but suppresses a warning */ \
  }

#define \
  VIDEO_IV_FCN_CALL_CONST(member_function, ...) { \
    switch(vtype()) { \
      case Video_Base::ZENI_VIDEO_GL: return reinterpret_cast<const Video_GL *>(this)->member_function(__VA_ARGS__); \
      case Video_Base::ZENI_VIDEO_DX9: return reinterpret_cast<const Video_DX9 *>(this)->member_function(__VA_ARGS__); \
      default: abort(); /* implies ZENI_VIDEO_ANY, which *should* be impossible */ \
    } \
    exit(END_OF_TIME); /* cannot be called, but suppresses a warning */ \
  }

#else

#define \
  VIDEO_IV_FCN_CALL(member_function, ...) { \
    return reinterpret_cast<Video_DX9 *>(this)->member_function(__VA_ARGS__); \
  }

#define \
  VIDEO_IV_FCN_CALL_CONST(member_function, ...) { \
    return reinterpret_cast<const Video_DX9 *>(this)->member_function(__VA_ARGS__); \
  }

#endif
#else

#define \
  VIDEO_IV_FCN_CALL(member_function, ...) { \
    return reinterpret_cast<Video_GL *>(this)->member_function(__VA_ARGS__); \
  }

#define \
  VIDEO_IV_FCN_CALL_CONST(member_function, ...) { \
    return reinterpret_cast<const Video_GL *>(this)->member_function(__VA_ARGS__); \
  }

#endif

  void Video::render(const Renderable &renderable) {
    class PrePostRenderActor {
      PrePostRenderActor & operator=(const PrePostRenderActor &) {return *this;}

    public:
      PrePostRenderActor(const Renderable &renderable_)
        : renderable(renderable_)
      {
        renderable.pre_render();
      }

      ~PrePostRenderActor() {
        renderable.post_render();
      }
    private:
      const Renderable &renderable;
    } ppra(renderable);

    VIDEO_IV_FCN_CALL(render_impl, renderable);
  }

  int Video::get_maximum_anisotropy() const {
    VIDEO_IV_FCN_CALL_CONST(get_maximum_anisotropy_impl, );
  }

  bool Video::has_vertex_buffers() const {
    VIDEO_IV_FCN_CALL_CONST(has_vertex_buffers_impl, );
  }

  void Video::set_2d_view(const std::pair<Point2f, Point2f> &camera2d, const std::pair<Point2i, Point2i> &viewport) {
    const Matrix4f view = Matrix4f::Identity();
    set_view_matrix(view);

    const Point2f offset = get_pixel_offset();

    const Matrix4f projection = Matrix4f::Orthographic(camera2d.first.x + offset.x,
                                                       camera2d.second.x + offset.x,
                                                       camera2d.second.y + offset.y,
                                                       camera2d.first.y + offset.y,
                                                       ZENI_2D_NEAR, ZENI_2D_FAR);
    set_projection_matrix(projection);

    set_viewport(viewport);

    VIDEO_IV_FCN_CALL(set_2d_view_impl, camera2d, viewport);
  }

  void Video::set_3d_view(const Camera &camera, const std::pair<Point2i, Point2i> &viewport) {
    const Matrix4f view = camera.get_view_matrix();
    set_view_matrix(view);

    Matrix4f projection = camera.get_projection_matrix(viewport);
    set_projection_matrix(projection);

    set_viewport(viewport);

    VIDEO_IV_FCN_CALL(set_3d_view_impl, camera, viewport);
  }

  void Video::set_backface_culling(const bool &on) {
    g_backface_culling = on;

    VIDEO_IV_FCN_CALL(set_backface_culling_impl, on);
  }

  void Video::set_vertical_sync(const bool &on) {
    g_vertical_sync = on;

    VIDEO_IV_FCN_CALL(set_vertical_sync_impl, on);
  }

  void Video::set_zwrite(const bool &enabled) {
    m_zwrite = enabled;

    VIDEO_IV_FCN_CALL(set_zwrite_impl, enabled);
  }

  void Video::set_ztest(const bool &enabled) {
    m_ztest = enabled;

    VIDEO_IV_FCN_CALL(set_ztest_impl, enabled);
  }

  void Video::set_alpha_test(const bool &enabled,
                             const TEST &test,
                             const float &value) {
    m_alpha_test = enabled;
    m_alpha_function = test;
    m_alpha_value = value;

    VIDEO_IV_FCN_CALL(set_alpha_test_impl, enabled, test, value);
  }

  void Video::set_color(const Color &color) {
    m_color = color;

    VIDEO_IV_FCN_CALL(set_color_impl, color);
  }

  void Video::set_clear_color(const Color &color) {
    m_clear_color = color;

    VIDEO_IV_FCN_CALL(set_clear_color_impl, color);
  }

  void Video::apply_texture(const unsigned long &id) {
    VIDEO_IV_FCN_CALL(apply_texture_impl, id);
  }

  void Video::apply_texture(const Texture &texture) {
    VIDEO_IV_FCN_CALL(apply_texture_impl, texture);
  }

  void Video::unapply_texture() {
    VIDEO_IV_FCN_CALL(unapply_texture_impl, );
  }

  void Video::set_lighting(const bool &on) {
    g_lighting = on;

    VIDEO_IV_FCN_CALL(set_lighting_impl, on);
  }

  void Video::set_ambient_lighting(const Color &color) {
    VIDEO_IV_FCN_CALL(set_ambient_lighting_impl, color);
  }

  void Video::set_light(const int &number, const Light &light) {
    VIDEO_IV_FCN_CALL(set_light_impl, number, light);
  }

  void Video::unset_light(const int &number) {
    VIDEO_IV_FCN_CALL(unset_light_impl, number);
  }

  void Video::set_material(const Material &material) {
    VIDEO_IV_FCN_CALL(set_material_impl, material);
  }

  void Video::unset_material(const Material &material) {
    VIDEO_IV_FCN_CALL(unset_material_impl, material);
  }

  void Video::set_fog(const Fog &fog) {
    VIDEO_IV_FCN_CALL(set_fog_impl, fog);
  }

  void Video::unset_fog() {
    VIDEO_IV_FCN_CALL(unset_fog_impl, );
  }

#ifndef DISABLE_CG
  void Video::set_vertex_shader(const Vertex_Shader &shader) {
    VIDEO_IV_FCN_CALL(set_vertex_shader_impl, shader);
  }

  void Video::set_fragment_shader(const Fragment_Shader &shader) {
    VIDEO_IV_FCN_CALL(set_fragment_shader_impl, shader);
  }

  void Video::unset_vertex_shader(const Vertex_Shader &shader) {
    VIDEO_IV_FCN_CALL(unset_vertex_shader_impl, shader);
  }

  void Video::unset_fragment_shader(const Fragment_Shader &shader) {
    VIDEO_IV_FCN_CALL(unset_fragment_shader_impl, shader);
  }
#endif

  void Video::set_render_target(Texture &texture) {
    VIDEO_IV_FCN_CALL(set_render_target_impl, texture);
  }

  void Video::unset_render_target() {
    VIDEO_IV_FCN_CALL(unset_render_target_impl, );
  }

  void Video::clear_render_target(const Color &color) {
    VIDEO_IV_FCN_CALL(clear_render_target_impl, color);
  }

  const Point2i & Video::get_render_target_size() const {
    VIDEO_IV_FCN_CALL_CONST(get_render_target_size_impl, );
  }

  void Video::select_world_matrix() {
    VIDEO_IV_FCN_CALL(select_world_matrix_impl, );
  }

  void Video::push_world_stack() {
    VIDEO_IV_FCN_CALL(push_world_stack_impl, );
  }

  void Video::pop_world_stack() {
    VIDEO_IV_FCN_CALL(pop_world_stack_impl, );
  }

  void Video::translate_scene(const Vector3f &direction) {
    VIDEO_IV_FCN_CALL(translate_scene_impl, direction);
  }

  void Video::rotate_scene(const Vector3f &about, const float &radians) {
    VIDEO_IV_FCN_CALL(rotate_scene_impl, about, radians);
  }

  void Video::scale_scene(const Vector3f &factor) {
    VIDEO_IV_FCN_CALL(scale_scene_impl, factor);
  }

  void Video::transform_scene(const Matrix4f &transformation) {
    VIDEO_IV_FCN_CALL(transform_scene_impl, transformation);
  }

  Point2f Video::get_pixel_offset() const {
    VIDEO_IV_FCN_CALL_CONST(get_pixel_offset_impl, );
  }

  void Video::set_view_matrix(const Matrix4f &view) {
    m_view = view;

    VIDEO_IV_FCN_CALL(set_view_matrix_impl, view);
  }

  void Video::set_projection_matrix(const Matrix4f &projection) {
    m_projection = projection;

    VIDEO_IV_FCN_CALL(set_projection_matrix_impl, projection);
  }

  void Video::set_viewport(const std::pair<Point2i, Point2i> &viewport) {
    m_viewport = viewport;

    VIDEO_IV_FCN_CALL(set_viewport_impl, viewport);
  }

  Texture * Video::load_Texture(const std::string &filename, const bool &repeat, const bool &lazy_loading) {
    VIDEO_IV_FCN_CALL(load_Texture_impl, filename, repeat, lazy_loading);
  }

  Texture * Video::create_Texture(SDL_Surface * const &surface, const bool &repeat) {
    VIDEO_IV_FCN_CALL(create_Texture_impl, surface, repeat);
  }

  Texture * Video::create_Texture(const Point2i &size, const bool &repeat) {
    VIDEO_IV_FCN_CALL(create_Texture_impl, size, repeat);
  }

  Font * Video::create_Font(const std::string &filename, const bool &bold, const bool &italic, 
    const float &glyph_height, const float &virtual_screen_height) {
    VIDEO_IV_FCN_CALL(create_Font_impl, filename, bold, italic, glyph_height, virtual_screen_height);
  }

  Vertex_Buffer * Video::create_Vertex_Buffer() {
    VIDEO_IV_FCN_CALL(create_Vertex_Buffer_impl, );
  }

#ifndef DISABLE_CG
  void Video::initialize(Shader_System &shader_system) {
    VIDEO_IV_FCN_CALL(initialize_impl, shader_system);
  }

  void Video::initialize(Vertex_Shader &shader, const std::string &filename, const std::string &entry_function) {
    VIDEO_IV_FCN_CALL(initialize_impl, shader, filename, entry_function);
  }

  void Video::initialize(Fragment_Shader &shader, const std::string &filename, const std::string &entry_function) {
    VIDEO_IV_FCN_CALL(initialize_impl, shader, filename, entry_function);
  }
#endif

  void Video::uninit() {
    SDL_FreeSurface(m_display_surface);
    m_display_surface = 0;
    g_initialized = false;

    VIDEO_IV_FCN_CALL(uninit_impl, );
  }

#undef VIDEO_IV_FCN_CALL
#undef VIDEO_IV_FCN_CALL_CONST
}

#include <Zeni/Global_Undef.h>

#include <Zeni/Camera.hxx>
#include <Zeni/Renderable.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Quaternion.hxx>
#include <Zeni/Video_DX9.hxx>
#include <Zeni/Video_GL.hxx>

#endif
