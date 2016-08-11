#include "logic.h"

struct BombInfo
{
  uint8_t d_time;  /* Detonate time. */
  struct Position pos;
};

/*============================*/
/*                            */
/* Global variables.          */
/*                            */
static struct Field g_field;
static struct StatsTable g_table;
static struct BombInfo g_bomb_info[MAX_PLAYER_AMOUNT];
static struct Position g_player_pos[MAX_PLAYER_AMOUNT];
static uint8_t g_res_time[MAX_PLAYER_AMOUNT];  /* Respawn time. */

/*============================*/
/*                            */
/* Helpfull functions.        */
/*                            */

/* Interface. */
static void RemoveSuicides(const struct ActionTable *action_table,
                           struct StatsTable **stats_table);

static int CanPlant(int player_num, struct Postion bomb_pos);

static void PlantBomb(struct Position pos);

static void PlantBombs(struct ActionTable *action_table);

static int CanMove(int player_num, struct Position next_pos);

static void MovePlayer(int player_num, struct Position next_pos);

static void MovePlayers(const struct ActionTable *action_table);

static void Boom(void);

static void DecreaseRespawn(void);

/* Definitons. */
static void RemoveSuicides(const struct ActionTable *action_table,
                           struct StatsTable **stats_table)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].suicide) {
      stats_table->player_stats[i].score  = 0;
      stats_table->player_stats[i].length = 0;
      stats_table->player_stats[i].death  = 0;
      stats_table->player_stats[i].bomb   = 0;
    }
  }
}

static int CanPlant(int player_num, struct Postion bomb_pos)
{
  return !(g_bomb_info[player_num].pos.x == bomb_pos.x &&
           g_bomb_info[player_num].pos.y == bomb_pos.y );
}

static void PlantBomb(struct Position pos)
{
  g_field[pos.y][pos.x] = BOMB;
}

static void PlantBombs(struct ActionTable *action_table)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].bomb_pos.x == 0) {
      continue
    }
    if (CanPlant(i, action_table->player_int[i].bomb_pos)) {
      PlantBomb(action_table->player_info[i].bomb_pos);
    }
  }
}

static void MovePlayer(int player_num, struct Position next_pos)
{
  
}

static void MovePlayers(const struct ActionTable *action_table)
{
  int i;

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    if (action_table->player_info[i].move_pos.x == 0) {
      continue;
    }

    if (CanMove(i, action_table->player_info[i].move_pos)) {
      MovePlayer(i, action_table->player_info[i].move_pos);
    }
  }
}

static void Boom(void)
{

}

static void DecreaseRespawn(void)
{

}

/*============================*/
/*                            */
/* Interface definitions.     */
/*                            */
void Update(const struct ActionTable *action_table,
            struct Field **game_field, struct StatsTable **stats_table)
{
  *game_field = &g_field;
  RemoveSuicides(action_table, stats_table);
  PlantsBombs(action_table);
  MovePlayers(action_table);
  Boom();
  DecreaseRespawn();
}

void SetUp(struct Field **game_field,
           struct StatsTanle **stats_table)
{
  uint16_t i, j;
  enum Cell **field;
  *game_field = 0;
  *stats_table = 0;

  /* Code below instantiate 2-dimentional array
   * First dimention is an array of pointers
   * Second dimention is an linear buffer of enum Cell */
  g_field.location = calloc(sizeof(void *) * FIELD_SIZE);
  if (g_field.location == 0) {
    perror("Field alloc(1) error");
    return;
  }

  g_field.location[0] = calloc(sizeof(enum Cell) * FIELD_SIZE * FIELD_SIZE);
  if (g_field.location == 0) {
    perror("Field alloc(2) error");
    return;
  }

  /* Filling field
   * FIXME : move to standalone function */

  for (i = 1; i < FIELD_SIZE; ++i) {
    g_field.location[i] = g_field.location[0] + FIELD_SIZE * i;
  }

  field = g_field.location;
  for (i = 1; i < FIELD_SIZE; i += 2) {
    for (j = 1; j < FIELD_SIZE; j += 2) {
      field[i][j] = WALL;
    }
  }

  for (i = 0; i < FIELD_SIZE; i += 2) {
    for (j = 0; j < FIELD_SIZE; j += 2) {
      field[i][j] = BOX;
    }
  }

  field[0][0] = EMPTY;
  field[0][1] = EMPTY;
  field[1][0] = EMPTY;

  field[FIELD_SIZE - 1][0] = EMPTY;
  field[FIELD_SIZE - 1][1] = EMPTY;
  field[FIELD_SIZE - 2][0] = EMPTY;

  field[FIELD_SIZE - 1][FIELD_SIZE - 1] = EMPTY;
  field[FIELD_SIZE - 1][FIELD_SIZE - 2] = EMPTY;
  field[FIELD_SIZE - 2][FIELD_SIZE - 1] = EMPTY;

  field[0][FIELD_SIZE - 1] = EMPTY;
  field[0][FIELD_SIZE - 2] = EMPTY;
  field[1][FIELD_SIZE - 1] = EMPTY;

  *game_field = &g_field;

  /* Init stats table */
  memset(g_table.player_stats, 0, sizeof(struct Dklb) * MAX_PLAYER_AMOUNT);

  for (i = 0; i < MAX_PLAYER_AMOUNT; ++i) {
    g_table.player_stats[0].bomb = 1;
    g_table.player_stats[0].lenght = 1;
  }

  *stats_table = &g_table;
}
