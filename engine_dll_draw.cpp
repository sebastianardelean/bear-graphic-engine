#include "framework.h"
#include "configuration.h"
#include "engine_types.h"

#include "debug.h"

#define NORMALIZE_3D(in, out) \
  do { \
  (out).x = 2 *((in).x/(float)SCREEN_WIDTH) - 1;           \
  (out).y = 2 *((in).y/(float)SCREEN_HEIGHT) - 1;          \
  (out).z = (in).z;                                         \
  } while (0)

/* Functions defined in engine_dll_texture.c */

extern void EngineLoadSpriteFromResource(
  const DWORD dwResourceId,
  const flip_t flip,
  sprite_t *sprite
  );


/* Functions defined in engine_dll_draw.c */
extern void EngineDrawPoint(
    const coordinate_t p,
    const color_t color
    );

extern void EngineDrawLine(
    const coordinate_t p1,
    const coordinate_t p2,
    const color_t color
    );

extern void EngineDrawTriangle(
    const coordinate_t top_p,
    const coordinate_t bottom_left_p,
    const coordinate_t bottom_right_p,
    const color_t color
    );

extern void EngineDrawQuad(
    const coordinate_t top_left_p,
    const coordinate_t top_right_p,
    const coordinate_t bottom_right_p,
    const coordinate_t bottom_left_p,
    const color_t color
    );

extern void EngineDrawSprite(
    const INT32 x,
    const INT32 y,
    const DWORD scale,
    const DWORD dwResourceId,
    const flip_t flip
    );


void EngineDrawPoint(
                            const coordinate_t p,
                            const color_t color)
{
  coordinate_t m_p;
  NORMALIZE_3D(p, m_p);
  glBegin(GL_POINTS);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p.x, m_p.y, m_p.z);
  glEnd();
}


void EngineDrawLine(
    const coordinate_t p1, 
    const coordinate_t p2, 
    const color_t color)
{

  coordinate_t m_p1;
  coordinate_t m_p2;
  NORMALIZE_3D(p1, m_p1);
  NORMALIZE_3D(p2, m_p2);
  
 


  glBegin(GL_LINES);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p1.x, m_p1.y, m_p1.z);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p2.x, m_p2.y, m_p2.z);
  glEnd();

}

void EngineDrawTriangle(
                        const coordinate_t top_p,
                        const coordinate_t bottom_left_p,
                        const coordinate_t bottom_right_p,
                        const color_t color
                        )
{
  coordinate_t m_p1;
  coordinate_t m_p2;
  coordinate_t m_p3;
  NORMALIZE_3D(top_p, m_p1);
  NORMALIZE_3D(bottom_left_p, m_p2);
  NORMALIZE_3D(bottom_right_p, m_p3);

  
  glBegin(GL_TRIANGLES);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p1.x, m_p1.y, m_p1.z);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p2.x, m_p2.y, m_p2.z);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p3.x, m_p3.y, m_p3.z);
  glEnd();
}


void EngineDrawQuad(
                    const coordinate_t top_left_p,
                    const coordinate_t top_right_p,
                    const coordinate_t bottom_right_p,
                    const coordinate_t bottom_left_p,
                    const color_t color
                    )
{
  coordinate_t m_p1;
  coordinate_t m_p2;
  coordinate_t m_p3;
  coordinate_t m_p4;
  
  NORMALIZE_3D(top_left_p, m_p1);
  NORMALIZE_3D(top_right_p, m_p2);
  NORMALIZE_3D(bottom_right_p, m_p3);
  NORMALIZE_3D(bottom_left_p, m_p4);


  glBegin(GL_QUADS);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p1.x, m_p1.y, m_p1.z);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p2.x, m_p2.y, m_p2.z);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p3.x, m_p3.y, m_p3.z);
  glColor4d(color.r/255.0f, color.g/255.0f, color.b/255.0f, color.a/255.0f);
  glVertex3f(m_p4.x, m_p4.y, m_p4.z);
  glEnd();
  
}

void EngineDrawSprite(
  const INT32 x,
  const INT32 y,
  const DWORD scale,
  const DWORD dwResourceId,
  const flip_t flip)
{
  sprite_t sprite;

  INT32 fxs = 0;
  INT32 fys = 0;
  INT32 fxm = 1;
  INT32 fym = 1;
  INT32 fx = 0;
  INT32 fy = 0;

  INT32 i = 0;
  INT32 j = 0;
  DWORD is = 0;
  DWORD js = 0;

  EngineLoadSpriteFromResource(dwResourceId,flip, &sprite);
#if 1
  for (i = 0; i < sprite.width; i++)
  {
    for (j = 0; j < sprite.height; j++)
    {
      coordinate_t p = {
        (float)(x + (i * scale)),
        (float)(y + (j * scale)),
        -2.0f
      };
      
      //DEBUG_W(L"Point coordinate %f %f", p.x, p.y);
      EngineDrawPoint(p,sprite.pixels[j*sprite.width+i].color); 
    }
  }
#endif  
#if 0
  if (sprite.flip == FLIP_HORIZONTAL)
  {
    fxs = sprite.width - 1;
    fxm = -1;
  }
  
  if (sprite.flip == FLIP_VERTICAL)
  {
    fys = sprite.height - 1;
    fym = -1;
  }

  if (scale > 1)
  {
    fx = fxs;
    for (i = 0; i < sprite.width; i++, fx += fxm)
    {
      fy = fys;
      for (j = 0; j < sprite.height; j++, fy += fym)
      {
        for (is = 0; is < scale; is++)
        {
          for (js = 0; js < scale; js++)
          {
            coordinate_t p = {
              (x + (i * scale) + is),
              (y + (j * scale) + js),
              -1
            };

            //DEBUG_W(L"Point coordinate %f %f", p.x, p.y);
            EngineDrawPoint(p,sprite.pixels[i*fx+fy]); 
          }
        }
      }
    }
  }
#endif
#if 0
    DEBUG_W(L"Sprite  %d x %d Data \n", sprite.width, sprite.height);


    for (i = 0; i < (sprite.width*sprite.height); i++)
    {
        DEBUG_W(L"%u %u %u", sprite.pixels[i].color.r,
                sprite.pixels[i].color.g, sprite.pixels[i].color.b);
    }
#endif
}
