/*
 * Thinker - AI improvement mod for Sid Meier's Alpha Centauri.
 * https://github.com/induktio/thinker/
 *
 * Thinker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) version 3 of the GPL.
 *
 * Thinker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Thinker.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef BUILD_REL
    #define MOD_VERSION "Thinker Mod v3.9"
#else
    #define MOD_VERSION "Thinker Mod develop build"
#endif

#ifdef BUILD_DEBUG
    #define MOD_DATE __DATE__ " " __TIME__
    #define DEBUG 1
    #define debug(...) fprintf(debug_log, __VA_ARGS__);
    #define debugf(...) { fprintf(debug_log, __VA_ARGS__); \
        fflush(debug_log); }
    #define flushlog() fflush(debug_log);
#else
    #define MOD_DATE __DATE__
    #define DEBUG 0
    #define NDEBUG /* Disable assertions */
    #define debug(...) /* Nothing */
    #define debugf(...) /* Nothing */
    #define flushlog()
    #ifdef __GNUC__
    #pragma GCC diagnostic ignored "-Wunused-variable"
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #endif
#endif

#ifdef __GNUC__
    #define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
    #pragma GCC diagnostic ignored "-Wchar-subscripts"
    #pragma GCC diagnostic ignored "-Wattributes"
#else
    #define UNUSED(x) UNUSED_ ## x
#endif

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <psapi.h>
#include <algorithm>
#include <set>
#include <list>
#include <string>
#include <vector>

#define DLL_EXPORT extern "C" __declspec(dllexport)
#define THINKER_HEADER (int16_t)0xACAC
#define ModAppName "thinker"
#define GameAppName "Alpha Centauri"
#define GameIniFile ".\\thinker.ini"

#ifdef BUILD_DEBUG
#ifdef assert
#undef assert
#define assert(_Expression) \
((!!(_Expression)) \
|| (fprintf(debug_log, "Assertion Failed: %s %s %d\n", #_Expression, __FILE__, __LINE__) \
&& (_assert(#_Expression, __FILE__, __LINE__), 0)))
#endif
#endif

const bool DEF = true;
const bool ATT = false;
const bool SEA = true;
const bool LAND = false;

const int MaxMapW = 512;
const int MaxMapH = 256;
const int MaxNaturalNum = 16;
const int MaxLandmarkNum = 64;
const int RegionBounds = 63;
const int MaxRegionNum = 128;
const int MaxRegionLandNum = 64;

const int MaxDiffNum = 6;
const int MaxPlayerNum = 8;
const int MaxGoalsNum = 75;
const int MaxSitesNum = 25;
const int MaxBaseNum = 512;
const int MaxVehNum = 2048;
const int MaxProtoNum = 512;
const int MaxProtoFactionNum = 64;
const int MaxBaseNameLen = 25;
const int MaxProtoNameLen = 32;

const int MaxTechnologyNum = 89;
const int MaxChassisNum = 9;
const int MaxWeaponNum = 26;
const int MaxArmorNum = 14;
const int MaxReactorNum = 4;
const int MaxAbilityNum = 29;

const int MaxFacilityNum = 134; // 0 slot unused
const int MaxSecretProjectNum = 64;
const int MaxSocialCatNum = 4;
const int MaxSocialModelNum = 4;
const int MaxSocialEffectNum = 11;
const int MaxMoodNum = 9;
const int MaxReputeNum = 8;
const int MaxMightNum = 7;
const int MaxBonusNameNum = 41;

const int StrBufLen = 256;
const int MaxEnemyRange = 50;

/*
Config parsed from thinker.ini. Alpha Centauri.ini related options
can be set negative values to use the defaults from Alpha Centauri.ini.
*/
struct Config {
    int directdraw = 0;
    int disable_opening_movie = 1;
    int autosave_interval = 1;
    int smooth_scrolling = 0;
    int scroll_area = 40;
    int render_base_info = 1;
    int render_high_detail = 1; // unlisted option
    int warn_on_former_replace = 1;
    int manage_player_bases = 0;
    int manage_player_units = 0;
    int render_probe_labels = 1;
    int foreign_treaty_popup = 0;
    int editor_free_units = 1;
    int new_base_names = 1;
    int new_unit_names = 1;
    int windowed = 0;
    int window_width = 1024;
    int window_height = 768;
    int smac_only = 0;
    int player_colony_pods = 0;
    int computer_colony_pods = 0;
    int player_formers = 0;
    int computer_formers = 0;
    int player_satellites[3] = {0,0,0};
    int computer_satellites[3] = {0,0,0};
    int design_units = 1;
    int factions_enabled = 7;
    int social_ai = 1;
    int social_ai_bias = 10;
    int tech_balance = 0;
    int base_hurry = 0;
    int base_spacing = 3;
    int base_nearby_limit = -1;
    int expansion_limit = 100;
    int expansion_autoscale = 0;
    // Adjust how often the AIs should build new military units instead of infrastructure.
    // Unlisted option for AI tuning. Allowed values 1-10000.
    int conquer_priority = 100;
    // Adjust how often the AIs should build new crawlers relative to the base count.
    // Unlisted option for AI tuning. Allowed values 1-10000.
    int crawler_priority = 100;
    int max_satellites = 20;
    int new_world_builder = 1;
    int world_sea_levels[3] = {45,57,70};
    int world_hills_mod = 40;
    int world_ocean_mod = 40;
    int world_islands_mod = 16;
    int world_continents = 0;
    int world_polar_caps = 1;
    int modified_landmarks = 0;
    int map_mirror_x = 0;
    int map_mirror_y = 0;
    int time_warp_mod = 1;
    int time_warp_techs = 5;
    int time_warp_projects = 1;
    int time_warp_start_turn = 40;
    int faction_placement = 1;
    int nutrient_bonus = 0;
    int rare_supply_pods = 0;
    int landmarks = 0xffff;
    int simple_cost_factor = 0;
    int revised_tech_cost = 1;
    int tech_cost_factor[MaxDiffNum] = {116,108,100,92,84,76};
    int cheap_early_tech = 1;
    int tech_stagnate_rate = 200;
    int fast_fungus_movement = 0;
    int magtube_movement_rate = 0;
    int road_movement_rate = 1; // internal variable
    int chopper_attack_rate = 1;
    int nerve_staple = 2;
    int nerve_staple_mod = -10;
    int delay_drone_riots = 0;
    int skip_drone_revolts = 1; // unlisted option
    int activate_skipped_units = 1; // unlisted option
    int counter_espionage = 0;
    int ignore_reactor_power = 0;
    int modify_unit_morale = 1; // unlisted option
    int skip_default_balance = 1; // unlisted option
    int early_research_start = 1; // unlisted option
    int facility_capture_fix = 1; // unlisted option
    int territory_border_fix = 1;
    int auto_relocate_hq = 1;
    int simple_hurry_cost = 1;
    int eco_damage_fix = 1;
    int clean_minerals = 16;
    int spawn_fungal_towers = 1;
    int spawn_spore_launchers = 1;
    int spawn_sealurks = 1;
    int spawn_battle_ogres = 1;
    int planetpearls = 1;
    int event_perihelion = 1;
    int event_sunspots = -1;
    int aquatic_bonus_minerals = 1;
    int alien_guaranteed_techs = 1;
    int alien_early_start = 0;
    int cult_early_start = 0;
    int natives_weak_until_turn = -1;
    int native_lifecycle_levels[6] = {40,80,120,160,200,240};
    int neural_amplifier_bonus = 50;
    int dream_twister_bonus = 50;
    int fungal_tower_bonus = 50;
    int planet_defense_bonus = 1;
    int perimeter_defense_bonus = 2;
    int tachyon_field_bonus = 2;
    int collateral_damage_value = 3;
    int content_pop_player[MaxDiffNum] = {6,5,4,3,2,1};
    int content_pop_computer[MaxDiffNum] = {3,3,3,3,3,3};
    int repair_minimal = 1;
    int repair_fungus = 2;
    int repair_friendly = 1;
    int repair_airbase = 1;
    int repair_bunker = 1;
    int repair_base = 1;
    int repair_base_native = 10;
    int repair_base_facility = 10;
    int repair_nano_factory = 10;
    int cpu_idle_fix = 1; // unlisted option
    int minimal_popups = 0; // unlisted option
    int skip_random_factions = 0; // internal variable
    int faction_file_count = 14; // internal variable
    int reduced_mode = 0; // internal variable
    int debug_mode = DEBUG; // internal variable
    int debug_verbose = DEBUG; // internal variable
};

/*
AIPlans contains several general purpose variables for AI decision-making
that are recalculated each turn. These values are not stored in the save game.
*/
struct AIPlans {
    int main_region = -1;
    int main_region_x = -1;
    int main_region_y = -1;
    int main_sea_region = -1;
    int target_land_region = 0;
    int prioritize_naval = 0;
    int naval_score = INT_MIN;
    int naval_scout_x = -1;
    int naval_scout_y = -1;
    int naval_airbase_x = -1;
    int naval_airbase_y = -1;
    int naval_start_x = -1;
    int naval_start_y = -1;
    int naval_end_x = -1;
    int naval_end_y = -1;
    int naval_beach_x = -1;
    int naval_beach_y = -1;
    int defend_weights = 0;
    int land_combat_units = 0;
    int sea_combat_units = 0;
    int air_combat_units = 0;
    int probe_units = 0;
    int missile_units = 0;
    int transport_units = 0;
    int unknown_factions = 0;
    int contacted_factions = 0;
    int enemy_factions = 0;
    int diplo_flags = 0;
    /*
    Amount of minerals a base needs to produce before it is allowed to build secret projects.
    All faction-owned bases are ranked each turn based on the surplus mineral production,
    and only the top third are selected for project building.
    */
    int project_limit = 5;
    int median_limit = 5;
    /*
    PSI combat units are only selected for production if this score is higher than zero.
    Higher values will make the prototype picker choose these units more often.
    */
    int psi_score = 0;
    int keep_fungus = 0;
    int plant_fungus = 0;
    int satellite_goal = 0;
    int enemy_odp = 0;
    int enemy_sat = 0;
    int enemy_nukes = 0;
    int mil_strength = 0;
    float enemy_base_range = 0;
    float enemy_mil_factor = 0;
    /*
    Number of our bases captured by another faction we're currently at war with.
    Important heuristic in threat calculation.
    */
    int enemy_bases = 0;
};

enum ResType {
    RES_NONE = 0,
    RES_NUTRIENT = 1,
    RES_MINERAL = 2,
    RES_ENERGY = 3,
};

enum NodesetType {
    NODE_CONVOY, // Resource being crawled
    NODE_BOREHOLE, // Borehole being built
    NODE_RAISE_LAND, // Land raise action initiated
    NODE_SENSOR_ARRAY, // Sensor being built
    NODE_NEED_FERRY,
    NODE_BASE_SITE,
    NODE_GOAL_RAISE_LAND, // Former priority only
    NODE_GOAL_NAVAL_START,
    NODE_GOAL_NAVAL_BEACH,
    NODE_GOAL_NAVAL_END,
    NODE_PATROL, // Includes probes/transports
    NODE_COMBAT_PATROL, // Only attack-capable units
};

#include "engine.h"
#include "strings.h"
#include "faction.h"
#include "random.h"
#include "patch.h"
#include "path.h"
#include "plan.h"
#include "gui.h"
#include "gui_dialog.h"
#include "veh.h"
#include "veh_combat.h"
#include "map.h"
#include "base.h"
#include "game.h"
#include "goal.h"
#include "move.h"
#include "tech.h"
#include "test.h"
#include "debug.h"

extern FILE* debug_log;
extern Config conf;
extern NodeSet mapnodes;
extern AIPlans plans[MaxPlayerNum];

DLL_EXPORT DWORD ThinkerModule();
int opt_list_parse(int* ptr, char* buf, int len, int min_val);



