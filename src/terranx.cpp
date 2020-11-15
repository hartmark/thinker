
#include "terranx.h"

BASE** current_base_ptr = (BASE**)0x90EA30;
int* current_base_id = (int*)0x689370;
int* game_settings = (int*)0x9A6490;
int* game_state = (int*)0x9A64C0;
int* game_rules = (int*)0x9A649C;
int* diff_level = (int*)0x9A64C4;
int* smacx_enabled = (int*)0x9A6488;
int* human_players = (int*)0x9A64E8;
int* current_turn = (int*)0x9A64D4;
int* active_faction = (int*)0x9A6820;
int* total_num_bases = (int*)0x9A64CC;
int* total_num_vehicles = (int*)0x9A64C8;
int* map_random_seed = (int*)0x949878;
int* map_toggle_flat = (int*)0x94988C;
int* map_area_tiles = (int*)0x949884;
int* map_area_sq_root = (int*)0x949888;
int* map_axis_x = (int*)0x949870;
int* map_axis_y = (int*)0x949874;
int* map_half_x = (int*)0x68FAF0;
int* climate_future_change = (int*)0x9A67D8;
int* un_charter_repeals = (int*)0x9A6638;
int* un_charter_reinstates = (int*)0x9A663C;
int* gender_default = (int*)0x9BBFEC;
int* plurality_default = (int*)0x9BBFF0;
int* current_player_faction = (int*)0x939284;
int* multiplayer_active = (int*)0x93F660;
int* pbem_active = (int*)0x93A95C;
int* sunspot_duration = (int*)0x9A6800;
int* diplo_active_faction = (int*)0x93F7CC;
int* diplo_current_friction = (int*)0x93FA74;
int* diplo_opponent_faction = (int*)0x8A4164;
int* base_find_dist = (int*)0x90EA04;
int* veh_attack_flags = (int*)0x93E904;

const char** engine_version = (const char**)0x691870;
const char** engine_date = (const char**)0x691874;

byte* tx_tech_discovered = (byte*)0x9A6670;
int* tx_secret_projects = (int*)0x9A6514;
int* tx_cost_ratios = (int*)0x689378;
short (*tx_faction_rankings)[8] = (short (*)[8])(0x9A68AC);
MetaFaction* tx_metafactions = (MetaFaction*)0x946A50;
Faction* tx_factions = (Faction*)0x96C9E0;
BASE* tx_bases = (BASE*)0x97D040;
UNIT* tx_units = (UNIT*)0x9AB868;
VEH* tx_vehicles = (VEH*)0x952828;
MAP** tx_map_ptr = (MAP**)0x94A30C;

R_Basic* tx_basic = (R_Basic*)0x949738;
R_Tech* tx_techs = (R_Tech*)0x94F358;
R_Social* tx_social = (R_Social*)0x94B000;
R_Facility* tx_facility = (R_Facility*)0x9A4B68;
R_Ability* tx_ability = (R_Ability*)0x9AB538;
R_Chassis* tx_chassis = (R_Chassis*)0x94A330;
R_Citizen* tx_citizen = (R_Citizen*)0x946020;
R_Defense* tx_defense = (R_Defense*)0x94F278;
R_Reactor* tx_reactor = (R_Reactor*)0x9527F8;
R_Resource* tx_resource = (R_Resource*)0x945F50;
R_Terraform* tx_terraform = (R_Terraform*)0x691878;
R_Weapon* tx_weapon = (R_Weapon*)0x94AE60;

fp_7intstr* propose_proto = (fp_7intstr*)0x580860;
fp_4int* veh_init = (fp_4int*)0x5C03D0;
fp_1int* veh_skip = (fp_1int*)0x5C1D20;
fp_2int* veh_at = (fp_2int*)0x5BFE90;
fp_2int* veh_speed = (fp_2int*)0x5C1540;
fp_3int* zoc_any = (fp_3int*)0x5C89F0;
fp_1int* monolith = (fp_1int*)0x57A050;
fp_2int* action_build = (fp_2int*)0x4C96E0;
fp_3int* action_terraform = (fp_3int*)0x4C9B00;
fp_3int* terraform_cost = (fp_3int*)0x4C9420;
fp_2int* bonus_at = (fp_2int*)0x592030;
fp_2int* goody_at = (fp_2int*)0x592140;
fp_3int* cost_factor = (fp_3int*)0x4E4430;
fp_3int* site_set = (fp_3int*)0x591B50;
fp_3int* world_site = (fp_3int*)0x5C4FD0;
fp_1int* set_base = (fp_1int*)0x4E39D0;
fp_1int* base_compute = (fp_1int*)0x4EC3B0;
fp_4int* base_prod_choices = (fp_4int*)0x4F81A0;
fp_void* turn_upkeep = (fp_void*)0x5258C0;
fp_1int* faction_upkeep = (fp_1int*)0x527290;
fp_1int* action_staple = (fp_1int*)0x4CA7F0;
fp_1int* social_upkeep = (fp_1int*)0x5B44D0;
fp_1int* repair_phase = (fp_1int*)0x526030;
fp_1int* production_phase = (fp_1int*)0x526E70;
fp_1int* allocate_energy = (fp_1int*)0x5267B0;
fp_1int* enemy_diplomacy = (fp_1int*)0x55F930;
fp_1int* enemy_strategy = (fp_1int*)0x561080;
fp_1int* corner_market = (fp_1int*)0x59EE50;
fp_1int* call_council = (fp_1int*)0x52C880;
fp_3int* setup_player = (fp_3int*)0x5B0E00;
fp_2int* eliminate_player = (fp_2int*)0x5B3380;
fp_2int* can_call_council = (fp_2int*)0x52C670;
fp_void* do_all_non_input = (fp_void*)0x5FCB20;
fp_void* auto_save = (fp_void*)0x5ABD20;
fp_2int* parse_num = (fp_2int*)0x625E30;
fp_icii* parse_says = (fp_icii*)0x625EC0;
fp_ccici* popp = (fp_ccici*)0x48C0A0;
fp_3int* capture_base = (fp_3int*)0x50C510;
fp_1int* base_kill = (fp_1int*)0x4E5250;
fp_5int* crop_yield = (fp_5int*)0x4E6E50;
fp_6int* base_draw = (fp_6int*)0x55AF20;
fp_6int* base_find3 = (fp_6int*)0x4E3D50;
fp_3int* draw_tile = (fp_3int*)0x46AF40;
tc_2int* font_width = (tc_2int*)0x619280;
tc_4int* buffer_box = (tc_4int*)0x5E3203;
tc_3int* buffer_fill3 = (tc_3int*)0x5DFCD0;
tc_5int* buffer_write_l = (tc_5int*)0x5DCEA0;
fp_6int* social_ai = (fp_6int*)0x5B4790;
fp_1int* social_set = (fp_1int*)0x5B4600;
fp_1int* pop_goal = (fp_1int*)0x4EF090;
fp_1int* consider_designs = (fp_1int*)0x581260;
fp_3int* tech_val = (fp_3int*)0x5BCBE0;
fp_1int* tech_rate = (fp_1int*)0x5BE6B0;
fp_1int* tech_selection = (fp_1int*)0x5BE380;
fp_1int* enemy_move = (fp_1int*)0x56B5B0;
fp_3int* best_defender = (fp_3int*)0x5044D0;
fp_5int* battle_compute = (fp_5int*)0x501DA0;
fp_6int* battle_kill = (fp_6int*)0x505D80;
fp_7int* battle_fight_2 = (fp_7int*)0x506AF0;

