#include "stdafx.h"
#include "snork.h"
#include "snork_state_manager.h"
#include "../../../detail_path_manager_space.h"
#include "../../../detail_path_manager.h"
#include "../../../level.h"
#include "../monster_velocity_space.h"
#include "../../../sound_player.h"
#include "../control_animation_base.h"
#include "../control_movement_base.h"
#include "../../../PHMovementControl.h"

CSnork::CSnork() 
{
	StateMan = xr_new<CStateManagerSnork>	(this);
	com_man().add_ability(ControlCom::eControlJump);
	com_man().add_ability(ControlCom::eControlThreaten);
}

CSnork::~CSnork()
{
	xr_delete		(StateMan);
}

void CSnork::Load(LPCSTR section)
{
	inherited::Load			(section);

	anim().accel_load	(section);

	anim().AddReplacedAnim(&m_bDamaged, eAnimStandIdle,	eAnimStandDamaged);
	anim().AddReplacedAnim(&m_bDamaged, eAnimRun,		eAnimRunDamaged);
	anim().AddReplacedAnim(&m_bDamaged, eAnimWalkFwd,	eAnimWalkDamaged);
	anim().AddReplacedAnim(&m_bRunTurnLeft,		eAnimRun,		eAnimRunTurnLeft);
	anim().AddReplacedAnim(&m_bRunTurnRight,	eAnimRun,		eAnimRunTurnRight);

	SVelocityParam &velocity_none		= move().get_velocity(MonsterMovement::eVelocityParameterIdle);	
	SVelocityParam &velocity_turn		= move().get_velocity(MonsterMovement::eVelocityParameterStand);
	SVelocityParam &velocity_walk		= move().get_velocity(MonsterMovement::eVelocityParameterWalkNormal);
	SVelocityParam &velocity_run		= move().get_velocity(MonsterMovement::eVelocityParameterRunNormal);
	SVelocityParam &velocity_walk_dmg	= move().get_velocity(MonsterMovement::eVelocityParameterWalkDamaged);
	SVelocityParam &velocity_run_dmg	= move().get_velocity(MonsterMovement::eVelocityParameterRunDamaged);
	SVelocityParam &velocity_steal		= move().get_velocity(MonsterMovement::eVelocityParameterSteal);
	//SVelocityParam &velocity_drag		= move().get_velocity(MonsterMovement::eVelocityParameterDrag);

	anim().AddAnim(eAnimStandIdle,		"stand_idle_",			-1, &velocity_none,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimStandDamaged,	"stand_idle_damaged_",	-1, &velocity_none,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimWalkDamaged,	"stand_walk_damaged_",	-1,	&velocity_walk_dmg,	PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimRunDamaged,		"stand_run_damaged_",	-1,	&velocity_run_dmg,	PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimStandTurnLeft,	"stand_turn_ls_",		-1, &velocity_turn,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimStandTurnRight,	"stand_turn_rs_",		-1, &velocity_turn,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimWalkFwd,		"stand_walk_fwd_",		-1,	&velocity_walk,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimRun,			"stand_run_",			-1,	&velocity_run,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimAttack,			"stand_attack_",		-1, &velocity_turn,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimDie,			"stand_die_",			0,  &velocity_none,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimLookAround,		"stand_look_around_",	-1, &velocity_none,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimSteal,			"stand_steal_",			-1, &velocity_steal,	PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimEat,			"stand_eat_",			-1, &velocity_none,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimCheckCorpse,	"stand_check_corpse_",	-1,	&velocity_none,		PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");

	anim().AddAnim(eAnimRunTurnLeft,	"stand_run_look_left_",	 -1, &velocity_run,	PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");
	anim().AddAnim(eAnimRunTurnRight,	"stand_run_look_right_", -1, &velocity_run,	PS_STAND, "stand_fx_f", "stand_fx_b", "stand_fx_l", "stand_fx_r");

	anim().LinkAction(ACT_STAND_IDLE,	eAnimStandIdle);
	anim().LinkAction(ACT_SIT_IDLE,		eAnimStandIdle);
	anim().LinkAction(ACT_LIE_IDLE,		eAnimStandIdle);
	anim().LinkAction(ACT_WALK_FWD,		eAnimWalkFwd);
	anim().LinkAction(ACT_WALK_BKWD,	eAnimWalkFwd);
	anim().LinkAction(ACT_RUN,			eAnimRun);
	anim().LinkAction(ACT_EAT,			eAnimEat);
	anim().LinkAction(ACT_SLEEP,		eAnimStandIdle);
	anim().LinkAction(ACT_REST,			eAnimStandIdle);
	anim().LinkAction(ACT_DRAG,			eAnimStandIdle);
	anim().LinkAction(ACT_ATTACK,		eAnimAttack);
	anim().LinkAction(ACT_STEAL,		eAnimSteal);
	anim().LinkAction(ACT_LOOK_AROUND,	eAnimLookAround);

#ifdef DEBUG	
	anim().accel_chain_test		();
#endif

	PostLoad					(section);
}

void CSnork::reinit()
{
	inherited::reinit	();
	
	if(CCustomMonster::use_simplified_visual() ) return;
	move().load_velocity(*cNameSect(), "Velocity_JumpGround",MonsterMovement::eSnorkVelocityParameterJumpGround);
	com_man().load_jump_data("stand_attack_2_0",0, "stand_attack_2_1", "stand_somersault_0", u32(-1), MonsterMovement::eSnorkVelocityParameterJumpGround,0);

	start_threaten = false;
	com_man().set_threaten_data	("stand_threaten_0", 0.63f);
}

void CSnork::UpdateCL()
{
	inherited::UpdateCL();
	
#ifdef _DEBUG
	// test 
	CObject *obj = Level().CurrentEntity();
	if (!obj) return;
	const CCoverPoint *point = CoverMan->find_cover(obj->Position(), 10.f, 30.f);
	
	DBG().level_info(this).clear();
	if (point) {
		DBG().level_info(this).add_item	(point->position(),COLOR_RED);
		
		Fvector pos;
		pos.set(Position());
		pos.y+=5.f;

		DBG().level_info(this).add_item	(Position(),pos,COLOR_GREEN);
	}
#endif

}

void CSnork::CheckSpecParams(u32 spec_params)
{
	if ((spec_params & ASP_CHECK_CORPSE) == ASP_CHECK_CORPSE) {
		com_man().seq_run(anim().get_motion_id(eAnimCheckCorpse));	
	}

	if ((spec_params & ASP_STAND_SCARED) == ASP_STAND_SCARED) {
		anim().SetCurAnim(eAnimLookAround);
		return;
	}
}

void CSnork::HitEntityInJump(const CEntity *pEntity)
{
	SAAParam &params	= anim().AA_GetParams("stand_attack_2_1");
	HitEntity			(pEntity, params.hit_power, params.impulse, params.impulse_dir);
}

//////////////////////////////////////////////////////////////////////////
void CSnork::jump(const Fvector &position, float factor)
{
	com_man().script_jump	(position, factor);
	sound().play			(MonsterSound::eMonsterSoundAggressive);
}

bool CSnork::check_start_conditions(ControlCom::EControlType type)
{
	if (!inherited::check_start_conditions(type))	return false;

	return type != ControlCom::eControlThreaten;
}

void CSnork::on_activate_control(ControlCom::EControlType type)
{
	if (type == ControlCom::eControlThreaten) 
	{
		sound().play(MonsterSound::eMonsterSoundThreaten);
		//m_sound_start_threaten.play_at_pos(this, get_head_position(this));
	}
}
//////////////////////////////////////////////////////////////////////////


#ifdef DEBUG
#include "Actor.h"
#include "ai_object_location.h"
#include "../../../../xrEngine/xr_level_controller.h"
void CSnork::debug_on_key(u8 key)
{
}
#endif

