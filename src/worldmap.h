//  $Id$
// 
//  SuperTux
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef SUPERTUX_WORLDMAP_H
#define SUPERTUX_WORLDMAP_H

#include <vector>
#include <string>

#include "vector.h"
#include "musicref.h"
#include "screen/screen.h"

namespace WorldMapNS {

class Tile
{
public:
  Tile();
  ~Tile();
  
  Surface* sprite;

  // Directions in which Tux is allowed to walk from this tile
  bool north;
  bool east;
  bool south;
  bool west;

  /** Stop on this tile or walk over it? */
  bool stop;

  /** When set automatically turn directions when walked over such a
      tile (ie. walk smoothly a curve) */
  bool auto_walk;
};

class TileManager
{
private:
  typedef std::vector<Tile*> Tiles;
  Tiles tiles;

public:
  TileManager();
  ~TileManager();

  Tile* get(int i);
};

enum Direction { D_NONE, D_WEST, D_EAST, D_NORTH, D_SOUTH };

std::string direction_to_string(Direction d);
Direction   string_to_direction(const std::string& d);
Direction reverse_dir(Direction d);

class WorldMap;

class Tux
{
public:
  Direction back_direction;
private:
  WorldMap* worldmap;
  Surface* largetux_sprite;
  Surface* firetux_sprite;
  Surface* smalltux_sprite;

  Direction input_direction;
  Direction direction;
  Vector tile_pos;
  /** Length by which tux is away from its current tile, length is in
      input_direction direction */
  float offset;
  bool  moving;

  void stop();
public: 
  Tux(WorldMap* worldmap_);
  ~Tux();
  
  void draw(DrawingContext& context, const Vector& offset);
  void action(float elapsed_time);

  void set_direction(Direction d) { input_direction = d; }

  bool is_moving() const { return moving; }
  Vector get_pos();
  Vector get_tile_pos() const { return tile_pos; } 
  void  set_tile_pos(Vector p) { tile_pos = p; } 
};

/** */
class WorldMap
{
private:
  Tux* tux;

  bool quit;

  Surface* level_sprite;
  Surface* leveldot_green;
  Surface* leveldot_red;

  std::string name;
  std::string music;

  std::vector<int> tilemap;
  int width;
  int height;
  
  int start_x;
  int start_y;

  TileManager* tile_manager;

public:
  struct Level
  {
    int x;
    int y;
    std::string name;
    std::string title;
    bool solved;

    /** Optional flags: */

    /** Check if this level should be vertically flipped */
    bool vertical_flip;

    /** Filename of the extro text to show once the level is
        successfully completed */
    std::string extro_filename;

    /** Position to swap player */
    int swap_x, swap_y;

    /** Message to show in the Map */
    std::string display_map_message;

    /** Go to this world */
    std::string next_worldmap;

    /** Quit the worldmap */
    bool quit_worldmap;

    // Directions which are walkable from this level
    bool north;
    bool east;
    bool south;
    bool west;
  };

private:
  std::string map_filename;

  typedef std::vector<Level> Levels;
  Levels levels;

  MusicRef song;

  Direction input_direction;
  bool enter_level;

  Vector offset;
  std::string savegame_file;

  void get_level_title(Level& level);

  void draw_status(DrawingContext& context);
public:
  WorldMap();
  ~WorldMap();

  /** Busy loop */
  void display();

  void load_map();
  
  void get_input();

  /** Update Tux position */
  void update(float delta);

  /** Draw one frame */
  void draw(DrawingContext& context, const Vector& offset);

  Vector get_next_tile(Vector pos, Direction direction);
  Tile* at(Vector pos);
  WorldMap::Level* at_level();

  /** Check if it is possible to walk from \a pos into \a direction,
      if possible, write the new position to \a new_pos */
  bool path_ok(Direction direction, Vector pos, Vector* new_pos);

  /* Save map to slot */
  void savegame(const std::string& filename);
  /* Load map from slot */
  void loadgame(const std::string& filename);
  /* Load map directly from file */
  void loadmap(const std::string& filename);

  const std::string& get_world_title() const
    { return name; }
    
  const int& get_start_x() const
    { return start_x; }
  
  const int& get_start_y() const
    { return start_y; }

private:
  void on_escape_press();
};

} // namespace WorldMapNS

#endif

/* Local Variables: */
/* mode:c++ */
/* End: */
