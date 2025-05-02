#include <DxLib.h>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

}

void ResourceManager::InitTitle(void)
{
	Resource res;

	//スカイドーム
	res = Resource(Resource::TYPE::MODEL, Application::PATH_SKYDOME + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	//タイトルロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);

	//キー指示メッセージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "PleaseKey.png");
	resourcesMap_.emplace(SRC::PLEASE_KEY, res);

	//タイトルBGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "TitleBGM.m4a");
	resourcesMap_.emplace(SRC::TITLE_BGM, res);

	//シーンチェンジ
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "SceneChange.mp3");
	resourcesMap_.emplace(SRC::SCENE_CHANGE_SE1, res);

}

void ResourceManager::InitSelect(void)
{
	Resource res;

	//人数選択画像
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "PlayerNumber.png", 4, 1, 300, 300);
	resourcesMap_.emplace(SRC::PLAYER_NUM, res);
	
	//画面数選択画像
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "DisPlayNumber.png", 4, 1, 300, 300);
	resourcesMap_.emplace(SRC::DISPLAY_NUM, res);

	//右矢印
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Right_point.png", 1, 1, 52, 52);
	resourcesMap_.emplace(SRC::RIGHT_POINT, res);

	//左矢印
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Left_point.png", 1, 1, 52, 52);
	resourcesMap_.emplace(SRC::LEFT_POINT, res);

	//準備
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Ready.png", 1, 1, 500, 200);
	resourcesMap_.emplace(SRC::READY, res);

	//役職
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "RoleNum.png", 1, 1, 300, 300);
	resourcesMap_.emplace(SRC::ROLE, res);

	//デバイス
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Device.png", 1, 1, 300, 300);
	resourcesMap_.emplace(SRC::DEVICE, res);

	//ComingSoon
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Coming Soon.png", 1, 1, 410, 200);
	resourcesMap_.emplace(SRC::COMING_SOON, res);

	//キャラクターパラメータリスト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "CharaParams.png", CHARA_PARAM_NUM_X, CHARA_PARAM_NUM_Y, CHARA_PARAM_SIZE_X, CHARA_PARAM_SIZE_Y);
	resourcesMap_.emplace(SRC::CHARA_PARAMS, res);

	//チキン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "SelectBGM.mp3");
	resourcesMap_.emplace(SRC::SELECT_BGM, res);

	//準備完了
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "SceneChange.mp3");
	resourcesMap_.emplace(SRC::SCENE_CHANGE_SE1, res);

	//キャンセル
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "Select_Cancel.mp3");
	resourcesMap_.emplace(SRC::CANCEL_SELECT, res);

	//選択変更
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "Change_Object.mp3");
	resourcesMap_.emplace(SRC::CHANGE_SELECT, res);

	//選択決定
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "Select_Decide.mp3");
	resourcesMap_.emplace(SRC::DECIDE_SELECT, res);

	//説明用画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Display_Select.png");
	resourcesMap_.emplace(SRC::DISPLAY_IMG, res);
	//説明用画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Player_Select.png");
	resourcesMap_.emplace(SRC::PLAYER_IMG, res);
	//説明用画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Operation_Select.png");
	resourcesMap_.emplace(SRC::OPERATION_IMG, res);
	//説明用画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Role_Select.png");
	resourcesMap_.emplace(SRC::ROLE_IMG, res);
	//説明用画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Waiting.png");
	resourcesMap_.emplace(SRC::WAIT_IMG, res);
	ResourcePlayer();
	ResourceEnemy();
	ResourceStage();
}

void ResourceManager::InitGame(void)
{
	Resource res;

	//煙エフェクト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Smokes.png", SMOKE_NUM_X, SMOKE_NUM_Y, SMOKE_SIZE_X, SMOKE_SIZE_Y);
	resourcesMap_.emplace(SRC::SMOKE, res);

	//ダメージエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Damage.efkefc");
	resourcesMap_.emplace(SRC::DAMAGE_EFE, res);

	//ゲームUI用数字画像2
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Numbers2.png", NUMBERS_NUM_X, NUMBERS_NUM_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS2, res);

	//パラメーターUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ParamUI.png");
	resourcesMap_.emplace(SRC::PARAM_UI, res);

	//ボスHP
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BossHpBar.png");
	resourcesMap_.emplace(SRC::HP_BOSS, res);

	//ボスHPBar
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BossHpGage.png");
	resourcesMap_.emplace(SRC::HP_BOSS_GAGE, res);

	//キャラアイコン
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "CharaIcons.png", CHARA_ICON_NUM_X, CHARA_ICON_NUM_Y, CHARA_ICON_SIZE, CHARA_ICON_SIZE);
	resourcesMap_.emplace(SRC::CHARA_ICONS, res);

	//キャラ名前
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "RoleNames.png", NAME_NUM_X, NAME_NUM_Y, NAME_SIZE_X, NAME_SIZE_Y);
	resourcesMap_.emplace(SRC::ROLE_NAMES, res);

	//タイマー背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TimerBack.png");
	resourcesMap_.emplace(SRC::TIMER_BACK, res);

	//BGM
	//ゲームノーマル
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Game_Nomal.mp3");
	resourcesMap_.emplace(SRC::GAME_NOMAL_BGM, res);
	//ゲームボス
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Game_Boss.mp3");
	resourcesMap_.emplace(SRC::GAME_LAST_BGM, res);

	//効果音
	//攻撃喰らい
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "hit.mp3");
	resourcesMap_.emplace(SRC::HIT_SND, res);
	//死亡(プレイヤー)
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "PlayerDeth.mp3");
	resourcesMap_.emplace(SRC::PLAYER_DETH_SND, res);
	//死亡(敵)
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "EnemyDeth.mp3");
	resourcesMap_.emplace(SRC::ENEMY_DETH_SND, res);
	//死亡(ニワトリ)
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "ChickenDeth.mp3");
	resourcesMap_.emplace(SRC::CHICKEN_DETH_SND, res);

	//強化選択用UI画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectUI.png");
	resourcesMap_.emplace(SRC::SELECT_UI, res);

	//カーソルまとめ画像(強化選択用)
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Carsors.png", CARSORS_NUM_X, CARSORS_NUM_Y, CARSORS_SIZE, CARSORS_SIZE);
	resourcesMap_.emplace(SRC::CARSOLS, res);

	//経験値ゲージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "CircleGage.png");
	resourcesMap_.emplace(SRC::CIRCLE_GAGE, res);

	//経験値経験値ゲージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "CircleExpGage.png");
	resourcesMap_.emplace(SRC::CIRCLE_EXP_GAGE, res);

	//ミッション①
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Mission_Nomal.png");
	resourcesMap_.emplace(SRC::MISSION_NOMAL, res);

	//ミッション②
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Mission_Last.png");
	resourcesMap_.emplace(SRC::MISSION_LAST, res);


	//ゲームUI用数字画像
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Numbers.png", NUMBERS_NUM_X,NUMBERS_NUM_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS, res);

	//レベル画面用エフェクト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "LevelUpEffect.png", LV_EFE_NUM_X, LV_EFE_NUM_Y, LV_EFE_SIZE, LV_EFE_SIZE);
	resourcesMap_.emplace(SRC::LEVEL_SCREEN_EFE, res);

	//レベルアップ背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "LevelUpBack.png");
	resourcesMap_.emplace(SRC::LEVEL_UP_BACK, res);

	//強化系エフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "LifeUp.efkefc");
	resourcesMap_.emplace(SRC::LIFE_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "AttackUp.efkefc");
	resourcesMap_.emplace(SRC::ATTACK_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "DefenceUp.efkefc");
	resourcesMap_.emplace(SRC::DEFENCE_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "SpeedUp.efkefc");
	resourcesMap_.emplace(SRC::SPEED_UP_EFE, res);

	//強化UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "AttackUp.png");
	resourcesMap_.emplace(SRC::ATTACK_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DefenseUp.png");
	resourcesMap_.emplace(SRC::DEFENCE_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SpeedUp.png");
	resourcesMap_.emplace(SRC::SPEED_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "LifeUp.png");
	resourcesMap_.emplace(SRC::LIFE_UP_UI, res);

	//HP関係UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpGage.png");
	resourcesMap_.emplace(SRC::HP_GAGE, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarPlayer.png");
	resourcesMap_.emplace(SRC::HP_PLAYER, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarEnemy.jpg");
	resourcesMap_.emplace(SRC::HP_ENEMY, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarCpu.jpg");
	resourcesMap_.emplace(SRC::HP_CPU, res);

	//時間
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "time.png");
	resourcesMap_.emplace(SRC::TIME, res);


	ResourcePlayer();

	//敵関連のエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "EnemyEffect/StateDownSpell.efkefc");
	resourcesMap_.emplace(SRC::STATE_DOWN_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "EnemyEffect/BossPunch.efkefc");
	resourcesMap_.emplace(SRC::BOSS_PUNCH_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "EnemyEffect/BossShout.efkefc");
	resourcesMap_.emplace(SRC::BOSS_SHOUT_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "EnemyEffect/BossShoutAtk.efkefc");
	resourcesMap_.emplace(SRC::BOSS_SHOUT_ATK_EFE, res);

	ResourceEnemy();
	ResourceStage();

	//チキン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

	//ヘルプ画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Help!.png");
	resourcesMap_.emplace(SRC::HELP, res);

	//ランクリスト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Ranks.png", RANKS_NUM_X, RANKS_NUM_Y, RANK_SIZE, RANK_SIZE);
	resourcesMap_.emplace(SRC::RANKS, res);

	//リザルト背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "FazeRezaltBack.jpg");
	resourcesMap_.emplace(SRC::REZALT_BACK, res);

	//煙エフェクト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Smokes.png", SMOKE_NUM_X, SMOKE_NUM_Y, SMOKE_SIZE_X, SMOKE_SIZE_Y);
	resourcesMap_.emplace(SRC::SMOKE, res);

	//ダメージエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Damage.efkefc");
	resourcesMap_.emplace(SRC::DAMAGE_EFE, res);

	//ゲームUI用数字画像2
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Numbers2.png", NUMBERS_NUM_X, NUMBERS_NUM_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS2, res);

	//パラメーターUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ParamUI.png");
	resourcesMap_.emplace(SRC::PARAM_UI, res);

	//ボスHP
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BossHpBar.png");
	resourcesMap_.emplace(SRC::HP_BOSS, res);

	//ボスHPBar
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BossHpGage.png");
	resourcesMap_.emplace(SRC::HP_BOSS_GAGE, res);

	//キャラアイコン
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "CharaIcons.png", CHARA_ICON_NUM_X, CHARA_ICON_NUM_Y, CHARA_ICON_SIZE, CHARA_ICON_SIZE);
	resourcesMap_.emplace(SRC::CHARA_ICONS, res);

	//キャラ名前
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "RoleNames.png", NAME_NUM_X, NAME_NUM_Y, NAME_SIZE_X, NAME_SIZE_Y);
	resourcesMap_.emplace(SRC::ROLE_NAMES, res);

	//タイマー背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TimerBack.png");
	resourcesMap_.emplace(SRC::TIMER_BACK, res);

	//カーソルSE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "Carsol.mp3");
	resourcesMap_.emplace(SRC::CARSOL_SE, res);

	//レベルアップSE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "LevelUp.mp3");
	resourcesMap_.emplace(SRC::LEVEL_UP_SE, res);
	//強化SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "PowerUp.mp3");
	resourcesMap_.emplace(SRC::POWER_UP_SE, res);

	//フェーズリザルトSE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "FazeRezalt.mp3");
	resourcesMap_.emplace(SRC::FAZE_REZALT_SE, res);

	//スキルUI関連
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "skill.png");
	resourcesMap_.emplace(SRC::SKILL_UI, res);
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "KnightSkills.png", SKILL_ICON_NUM_X, SKILL_ICON_NUM_Y, SKILL_ICON_SIZE, SKILL_ICON_SIZE);
	resourcesMap_.emplace(SRC::SKILL_KNIGHT, res);
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "AxemanSkills.png", SKILL_ICON_NUM_X, SKILL_ICON_NUM_Y, SKILL_ICON_SIZE, SKILL_ICON_SIZE);
	resourcesMap_.emplace(SRC::SKILL_AXEMAN, res);
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "ArcherSkills.png", SKILL_ICON_NUM_X, SKILL_ICON_NUM_Y, SKILL_ICON_SIZE, SKILL_ICON_SIZE);
	resourcesMap_.emplace(SRC::SKILL_ARCHER, res);

	//アーチャースキル2発動エフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Undine4.efkefc");
	resourcesMap_.emplace(SRC::ARCHER_SKILL2, res);

	//アーチャースキル2がプレイヤーにヒットしたときのエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Salamander.efkefc");
	resourcesMap_.emplace(SRC::BUFF_ARROW_HIT, res);
}

void ResourceManager::InitResult(void)
{
	Resource res;
	//ランクリスト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Ranks.png", RANKS_NUM_X, RANKS_NUM_Y, RANK_SIZE, RANK_SIZE);
	resourcesMap_.emplace(SRC::RANKS, res);

	//リザルト背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "FazeRezaltBack.jpg");
	resourcesMap_.emplace(SRC::REZALT_BACK, res);
}

void ResourceManager::InitGameOver(void)
{
	Resource res;

	//ゲームオーバーUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER, res);

	//ゲームオーバー背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOverBack.png");
	resourcesMap_.emplace(SRC::GAMEOVER_BACK, res);

	//タイトル遷移メッセージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ChangeTitleMes.png");
	resourcesMap_.emplace(SRC::CHANGE_TITLE_UI, res);

	//SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "GameOverSE.wav");
	resourcesMap_.emplace(SRC::GAMEOVER_SE, res);

	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "GameOverBGM.mp3");
	resourcesMap_.emplace(SRC::GAMEOVER_BGM, res);

	ResourcePlayer();
}

void ResourceManager::InitGameClear(void)
{
	Resource res;

	//祝福メッセージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Congratulations.png");
	resourcesMap_.emplace(SRC::CONGRATULATIONS, res);

	//タイトル遷移メッセージ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ChangeTitleMes.png");
	resourcesMap_.emplace(SRC::CHANGE_TITLE_UI, res);

	//花火
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Fireworks.efkefc");
	resourcesMap_.emplace(SRC::FIREWORK, res);

	//SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "GameClearSE.mp3");
	resourcesMap_.emplace(SRC::GAMECLEAR_SE, res);

	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "GameClearBGM.mp3");
	resourcesMap_.emplace(SRC::GAMECLEAR_BGM, res);

	ResourcePlayer();
	ResourceStage();

	//チキン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);
}

void ResourceManager::InitExplan(void)
{
	Resource res;

	//説明書
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan.png");
	resourcesMap_.emplace(SRC::EXPLAN, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan2.png");
	resourcesMap_.emplace(SRC::EXPLAN2, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan3.png");
	resourcesMap_.emplace(SRC::EXPLAN3, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan4.png");
	resourcesMap_.emplace(SRC::EXPLAN4, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan5.png");
	resourcesMap_.emplace(SRC::EXPLAN5, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan6.png");
	resourcesMap_.emplace(SRC::EXPLAN6, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan7.png");
	resourcesMap_.emplace(SRC::EXPLAN7, res);

	//左矢印
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Left_point.png", 1, 1, 52, 52);
	resourcesMap_.emplace(SRC::LEFT_POINT, res);

	//説明切り替え効果音
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "ExplanSwitch.mp3");
	resourcesMap_.emplace(SRC::EXPLAN_SWITCH, res);

	//シーンチェンジ
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "SceneChange.mp3");
	resourcesMap_.emplace(SRC::SCENE_CHANGE_SE1, res);
}

//プレイヤー
//*************************************************************************************************************
void ResourceManager::ResourcePlayer(void)
{
	Resource res;

	// エフェクト
		// ********************************************************************

	//斧使いのスキル1溜め攻撃
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "fire.efkproj");
	resourcesMap_.emplace(SRC::CHARGE_AXE_HIT, res);

	//騎士のガード
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Falloff.efkefc");
	resourcesMap_.emplace(SRC::GUARD, res);

	//アーチャーの矢のエフェクト(赤)
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Spear1.efkproj");
	resourcesMap_.emplace(SRC::ARROW_RED, res);

	//矢のエフェクト(白)
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Spear2.efkproj");
	resourcesMap_.emplace(SRC::ARROW_WHITE, res);

	//チャージエフェクト
	//res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Rock1.efkproj");
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Rock1.efkefc");
	resourcesMap_.emplace(SRC::CHARGE_SKILL, res);

	//当たりエフェクト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "hit_eff.efkproj");
	resourcesMap_.emplace(SRC::HIT2, res);


	//モデル
	//********************************************************************
	//魔法使い（プレイヤー）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Mage.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MAGE, res);

	//騎士（プレイヤー）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Knight.mv1");
	resourcesMap_.emplace(SRC::PLAYER_KNIGHT, res);

	//弓使い（プレイヤー）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Archer.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ARCHER, res);

	//斧使い（プレイヤー）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Axeman.mv1");
	resourcesMap_.emplace(SRC::PLAYER_AXEMAN, res);

	//効果音
	//**********************************************************************

	//スキル変更
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "SkillSwap.mp3");
	resourcesMap_.emplace(SRC::SKILL_CHANGE, res);
}

//敵
//*************************************************************************************************************
void ResourceManager::ResourceEnemy(void)
{
	Resource res;

	//魔法使い（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Mage.mv1");
	resourcesMap_.emplace(SRC::ENEMY_MAGE, res);

	//山賊（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Brigant.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BRIGANT, res);

	//弓使い（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Archer.mv1");
	resourcesMap_.emplace(SRC::ENEMY_ARCHER, res);

	//斧使い（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Axeman.mv1");
	resourcesMap_.emplace(SRC::ENEMY_AXEMAN, res);

	//ゴーレム（敵）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Boss_Golem.mv1");
	resourcesMap_.emplace(SRC::ENEMY_GOLEM, res);

	//矢
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ARROW + "Arrow.mv1");
	resourcesMap_.emplace(SRC::ARROW, res);
}

//ステージ
//*************************************************************************************************************
void ResourceManager::ResourceStage(void)
{
	Resource res;

	//ステージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Stage1.mv1");
	resourcesMap_.emplace(SRC::STAGE_01, res);

	//装飾
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Deco1.mv1");
	resourcesMap_.emplace(SRC::DECO_01, res);

	//スカイドーム
	res = Resource(Resource::TYPE::MODEL, Application::PATH_SKYDOME + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
