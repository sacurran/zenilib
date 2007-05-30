/* This file is part of the Zenipex Library.
* Copyleft (C) 2007 Mitchell Keith Bloch a.k.a. bazald
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

#include <Zeni/Matrix4f.hxx>

#include <Zeni/Core.h> // just for Error; should be removed

#include <cmath>

namespace Zeni {

  Matrix4f::Matrix4f()
  {
  }

  Matrix4f::Matrix4f(
    const float &aw, const float &ax, const float &ay, const float &az,
    const float &bw, const float &bx, const float &by, const float &bz,
    const float &cw, const float &cx, const float &cy, const float &cz,
    const float &dw, const float &dx, const float &dy, const float &dz)
  {
    m_matrix[0][0] = aw;
    m_matrix[0][1] = ax;
    m_matrix[0][2] = ay;
    m_matrix[0][3] = az;

    m_matrix[1][0] = bw;
    m_matrix[1][1] = bx;
    m_matrix[1][2] = by;
    m_matrix[1][3] = bz;

    m_matrix[2][0] = cw;
    m_matrix[2][1] = cx;
    m_matrix[2][2] = cy;
    m_matrix[2][3] = cz;

    m_matrix[3][0] = dw;
    m_matrix[3][1] = dx;
    m_matrix[3][2] = dy;
    m_matrix[3][3] = dz;
  }

  Matrix4f::Matrix4f(const Matrix4f &rhs)
  {
    *this = rhs;
  }

  Matrix4f & Matrix4f::invert()
  {
    return *this = inverted();
  }

  Matrix4f Matrix4f::inverted() const
  {
    throw Error("Invert no implemented yet... :(");
  }

  Matrix4f & Matrix4f::transpose()
  {
    for(int i = 1; i < 4; ++i)
      for(int j = 0; j < i; ++j) {
        float temp = m_matrix[i][j];
        m_matrix[i][j] = m_matrix[j][i];
        m_matrix[j][i] = temp;
      }

    return *this;
  }

  Matrix4f Matrix4f::transposed() const
  {
    Matrix4f matrix;

    for(int i = 0; i < 4; ++i)
      for(int j = 0; j < 4; ++j)
        matrix.m_matrix[i][j] = m_matrix[j][i];

    return matrix;
  }

  float Matrix4f::determinant() const
  {
#define m(i,j) m_matrix[i][j]
#define term(a,b,c,d) m(0,a)*m(1,b)*m(2,c)*m(3,d)

    return
      + term(0,1,2,3) - term(0,1,3,2)
      - term(0,2,1,3) + term(0,2,3,1)
      + term(0,3,1,2) + term(0,3,2,1)

      - term(1,0,2,3) + term(1,0,3,2)
      + term(1,2,0,3) - term(1,2,3,0)
      - term(1,3,0,2) + term(1,3,2,0)

      + term(2,0,1,3) - term(2,0,3,1)
      - term(2,1,0,3) + term(2,1,3,0)
      + term(2,3,0,1) - term(2,3,1,0)

      - term(3,0,1,2) + term(3,0,2,1)
      + term(3,1,0,2) - term(3,1,2,0)
      - term(3,2,0,1) + term(3,2,1,0);

#undef term
#undef m
  }
}
