#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Manager/Decoration/SoundManager.h"

#include"../Object/Character/EnemySort/Enemy.h"
#include"../Object/Character/EnemyManager.h"
#include"../Object/Character/PlayerManager.h"
#include"../Object/Character/Chiken/ChickenManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelScreenManager.h"
#include "../Object/System/UnitPositionLoad.h"
#include "../Manager/GameSystem/DataBank.h"
#include"../Object/Character/PlayerDodge.h"
#include"FazeResult.h"
#include "GameScene.h"

#include "../Manager/Generic/InputManager.h"



GameScene::GameScene(void)
{
	stage_ = nullptr;
	sky_ = nullptr;
	fader_ = nullptr;
	level_ = nullptr;
	chicken_ = nullptr;
	unitLoad_ = nullptr;
	fazeResult_ = nullptr;

	isPhaseChanging_ = false;
	fazeCnt_ = 1;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	//タイマーの生成
	Timer::CreateInstance();

	unitLoad_ = std::make_unique<UnitPositionLoad>();
	unitLoad_->Init();

	//ステージ
	stage_ = std::make_unique<StageManager>();
	stage_->Init();
	//スカイドーム
	sky_ = std::make_unique<SkyDome>();
	sky_->Init();
	//レベル関係
	level_ = std::make_unique<LevelScreenManager>();
	level_->Init();

	//プレイヤー設定
	playerMng_ = std::make_unique<PlayerManager>();
	playerMng_->Init();

	//敵マネージャの生成
	enmMng_ = std::make_unique<EnemyManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::ENEMY));
	enmMng_->Init();
	//ニワトリの生成
	chicken_ = std::make_unique<ChickenManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::CPU),
		stage_->GetTtans(),
		playerMng_->GetPlayer(0)->GetTransform());
	chicken_->Init();

	

	//カメラの設定
	auto cameras = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < cameras.size(); i++)
	{
		cameras[i]->SetFollow(&playerMng_->GetPlayer(i)->GetTransform());
		cameras[i]->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	}

	//フェーダーの作成
	fader_ = std::make_unique<Fader>();
	fader_->Init();
	//フェーズリザルトの作成
	fazeResult_ = std::make_unique<FazeResult>();
	fazeResult_->Init();
	missionImg_[static_cast<int>(FazeResult::STATE::NOMAL)]= ResourceManager::GetInstance().Load(ResourceManager::SRC::MISSION_NOMAL).handleId_;
	missionImg_[static_cast<int>(FazeResult::STATE::LAST)]= ResourceManager::GetInstance().Load(ResourceManager::SRC::MISSION_LAST).handleId_;

	//音声関係設定
	SoundInit();

}

void GameScene::Update(void)
{
	//ゲームオーバー判定
	if(IsGameOver())SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	if (!playerMng_->GetPlayer(0)->IsAlive())SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);

	//フェーズリザルト
	if (isFazeRezult_)
	{
		FazeResultUpdate();
		return;
	}

	//フェーズ遷移中
	if (isPhaseChanging_)
	{
		fader_->Update();
		//フェードをかける
		Fade();
		return;
	}
	//フェーズ数の知らせ
	if (isInformFaze_) {
		InformFazeNum();
		return;
	}

	//レベル処理
	level_->Update();
	//レベルアップ中その他の更新はかけない
	if (level_->IsLevelUp())return;
	//タイマー更新
	Timer::GetInstance().Update();
	//タイマーが終了したら
	if (Timer::GetInstance().IsEnd())ChangePhase();



	//プレイヤーの更新
	playerMng_->Update();


	//敵の更新
	enmMng_->Update();
	

	chicken_->SetTargetPos(playerMng_->GetPlayer(0)->GetPos());
	chicken_->Update();

	//あたり判定
	Collision();

	//強化要素の反映
	LevelUpReflection();



	//いずれ消す
	auto& ins = InputManager::GetInstance();
	auto& mng = SceneManager::GetInstance();
	////スペース推したらタイトルに戻る
	//if (ins.IsTrgDown(KEY_INPUT_SPACE))
	//{
	//	mng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	//}

	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		ChangePhase();
	}

	if (ins.IsTrgDown(KEY_INPUT_K))
		playerMng_->GetPlayer(0)->SetDamage(20, 20);
}

void GameScene::Draw(void)
{
	if (isFazeRezult_)
	{
		fazeResult_->Draw();
		return;
	}
	//スカイドーム
	sky_->Draw();

	//プレイヤー
	playerMng_->Draw();

	//敵
	enmMng_->Draw();
	//ステージ
	stage_->Draw();
	//チキン
	chicken_->Draw();
	//レベル
	level_->Draw();
	//制限時間
	Timer::GetInstance().Draw();

	fader_->Draw();

	if (fader_->GetState() == Fader::STATE::FADE_KEEP)
	{
		DrawPhase();
	}

}

void GameScene::Release(void)
{
	level_->Release();
	stage_->Destroy();

	SceneManager::GetInstance().ResetCameras();
	SceneManager::GetInstance().ReturnSolo();
	Timer::GetInstance().Reset();

	playerMng_->Release();

	enmMng_->Release();
}


void GameScene::SoundInit(void)
{
	//BGMの初期化
	auto& snd = SoundManager::GetInstance();

	//通常
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME_NOMAL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_NOMAL_BGM).handleId_);
	//ボス戦
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME_LAST,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_LAST_BGM).handleId_);

	//ゲームシーン開始時はノーマルのBGMを再生
	snd.Play(SoundManager::SOUND::GAME_NOMAL);

	//効果音設定
	//プレイヤー死亡
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_PLAYER,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_DETH_SND).handleId_);
	//敵死亡
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_ENEMY,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_DETH_SND).handleId_);
	//ニワトリ死亡
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_CHICKEN,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHICKEN_DETH_SND).handleId_);
	//攻撃喰らい時
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::HIT,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::HIT_SND).handleId_);
}

//当たり判定（他項目に干渉するもののみ）
//あたり判定総括
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();


	CollisionChicken();
	CollisionEnemy();
	CollisionPlayer();
}

//敵関係の当たり判定
void GameScene::CollisionEnemy(void)
{
	//衝突判定マネージャ取得
	auto& col = Collision::GetInstance();
	//敵の総数取得
	int maxCnt = enmMng_->GetActiveNum();

	bool isPlayerFound = false;

	enmMng_->CollisionStage(stage_->GetTtans());

	//あたり判定(主に索敵)
	for (int i = 0; i < maxCnt; i++)
	{
		//敵の取得
		Enemy* e = enmMng_->GetActiveEnemy(i);

		//敵個人の位置と攻撃を取得
		VECTOR ePos = e->GetPos();
		UnitBase::ATK eAtk = e->GetAtk();


		for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
		{
			PlayerBase* p = playerMng_->GetPlayer(i);
			VECTOR pPos = p->GetPos();

			//範囲内に入っているとき


			//通常状態時 && 攻撃範囲内にプレイヤーが入ったら攻撃を開始
			if (col.Search(ePos, pPos, e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//状態を変更
				e->ChangeState(Enemy::STATE::ALERT);
			}

			if (col.Search(ePos, pPos, e->GetSearchRange())) {
				//移動を開始
				e->SetIsMove(true);

				//プレイヤーを狙う
				e->ChangeSearchState(Enemy::SEARCH_STATE::PLAYER_FOUND);
				e->SetTargetPos(pPos);

				//見つけた
				isPlayerFound = true;
			}
			else if (!isPlayerFound) {
				//移動を停止
				e->SetIsMove(false);

				//誰も狙っていない
				e->ChangeSearchState(Enemy::SEARCH_STATE::NOT_FOUND);
				//e->SetTargetPos(VECTOR{0,0,0});
			}


			//攻撃判定

			//アタック中 && 攻撃判定が終了していないときだけ処理する。それ以外はしないので戻る
			if (eAtk.IsAttack() && !eAtk.isHit_) {


				//攻撃が当たる範囲 && プレイヤーが回避していないとき
				if (col.IsHitAtk(*e, *p)/* && !p->GetDodge()->IsDodge()*/)
				{
					//ダメージ
					p->SetDamage(e->GetCharaPow(), eAtk.pow_);
					//使用した攻撃を判定終了に
					e->SetIsHit(true);
				}
			}
		}	
	}
}

void GameScene::CollisionPlayer(void)
{
	//衝突判定マネージャ取得
	auto& col = Collision::GetInstance();
	//敵の総数取得
	int maxCnt = enmMng_->GetActiveNum();

	playerMng_->CollisionStage(stage_->GetTtans());

	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
	{
		PlayerBase* p = playerMng_->GetPlayer(i);

		auto pPos = p->GetPos();
		auto pAtk = p->GetAtk();

		//プレイヤーがCPUの時だけサーチしたい
		//if (p->GetPlayMode() == SceneManager::PLAY_MODE::CPU)CollisionPlayerCPU(*p, pPos);

		//プレイヤー攻撃判定
		//攻撃していない || 攻撃がすでに当たっている
		if (!pAtk.IsAttack() || pAtk.isHit_)continue;

		for (int i = 0; i < maxCnt; i++)
		{
			//敵の取得
			Enemy* e = enmMng_->GetActiveEnemy(i);

			//当たり判定
			if (col.IsHitAtk(*p, *e)) {
				//被弾
				e->SetDamage(p->GetCharaPow(), pAtk.pow_);
				e->Damage(e->GetDamage(), pAtk.pow_);

				//死んだら経験値増加
				if (!e->IsAlive())level_->AddExp(e->GetExp());

				//倒した敵の増加
				if (!e->IsAlive())dunkEnmCnt_++;
				//攻撃判定の終了
				p->SetIsHit(true);
			}
		}
		
	}
}

void GameScene::CollisionChicken(void)
{
	auto& col = Collision::GetInstance();

	int chickenNum = chicken_->GetChickenAllNum();

	for (int i = 0; i < chickenNum; i++) {
		auto c = chicken_->GetChicken(i);
		//ニワトリが死んでいたら次へ
 		if (!c->IsAlive())continue;

		//敵の総数取得
		int maxCnt = enmMng_->GetActiveNum();
		//攻撃判定

		for (int r = 0; r < maxCnt; r++)
		{

			//敵の取得
			Enemy* e = enmMng_->GetActiveEnemy(r);

			//敵個人の位置と攻撃を取得
			VECTOR ePos = e->GetPos();
			UnitBase::ATK eAtk = e->GetAtk();

			//索敵
			//範囲内に入っているとき
			if (col.Search(ePos, c->GetPos(), e->GetSearchRange())) {
				//移動を開始
				e->SetIsMove(true);
				//プレイヤーを狙う
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_FOUND);
				e->SetTargetPos(c->GetPos());
			}

			//通常状態時 && 攻撃範囲内にプレイヤーが入ったら攻撃を開始
			if (col.Search(ePos, c->GetPos(), e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//状態を変更
				e->ChangeState(Enemy::STATE::ALERT);
			}


			//アタック中 && 攻撃判定が終了していないときだけ処理する。それ以外はしないので戻る
			if (!(eAtk.IsAttack() && !eAtk.isHit_))continue;

			//攻撃が当たる範囲 && プレイヤーが回避していないとき
			if (col.IsHitAtk(*e, *c))
			{
				//ダメージ
				c->SetDamage(1);
				//使用した攻撃を判定終了に
				e->SetIsHit(true);
			}
		}
	}
}

void GameScene::CollisionPlayerCPU(PlayerBase& _player, const VECTOR& _pPos)
{
	////衝突判定マネージャ取得
	//auto& col = Collision::GetInstance();
	////敵の総数取得
	//int maxCnt = enmMng_->GetActiveNum();

	////敵をサーチ初期化
	//_player.SetisEnemySerch(false);


	////敵の個体分行う
	//for (int i = 0; i < maxCnt; i++)
	//{
	//	//敵の取得
	//	Enemy* e = enmMng_->GetActiveEnemy(i);

	//	//敵が死亡していたら処理しない
	//	if (!e->IsAlive())continue;

	//	//敵個人の位置と攻撃を取得
	//	VECTOR ePos = e->GetPos();

	//	//プレイヤー側索敵
	//	if (col.Search(_pPos, ePos, _player.GetSearchRange())
	//		&& !_player.GetIsCalledPlayer())
	//	{
	//		//敵をサーチしたかを返す
	//		_player.SetisEnemySerch(true);
	//		_player.SetTargetPos(ePos);
	//	}

	//	if (col.Search(_player.GetPos(), ePos, _player.GetAtkStartRange())
	//		&& _player.GetState() == PlayerBase::CPU_STATE::NORMAL
	//		&& !_player.GetIsCalledPlayer())
	//	{
	//		//状態を変更
	//		_player.ChangeState(PlayerBase::CPU_STATE::ATTACK);
	//	}
	//}
}

void GameScene::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())	//明転終了
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isPhaseChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())	//暗転終了
		{
			//ここの処理をフェーズ遷移がわかりやすいようなやつ始動に変える。
			// 暗転から明転へ
			//ある程度完全暗転の時間を用意
			fader_->SetFade(Fader::STATE::FADE_KEEP);

			//フェーズリザルトの状態に
			isFazeRezult_ = true;
		}
		break;
	case Fader::STATE::FADE_KEEP:
		UpdatePhase();
		break;
	default:
		break;
	}
}

//フェーズ関係
//*********************************************************
void GameScene::ChangePhase(void)
{
	//BGMの停止
	if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_LAST);
	else SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_NOMAL);

	//リザルトに関係するデータを入力
	DataBank& data = DataBank::GetInstance();

	data.Input(DataBank::INFO::FAZE_DUNK_ENEMY, dunkEnmCnt_);	//倒した敵数
	data.Input(DataBank::INFO::ALIVE_CHICKEN, chicken_->GetAliveNum());		//ニワトリ生存数

	//リザルトで取得
	fazeResult_->SetResult();
	if (fazeCnt_ == LAST_FAZE)fazeResult_->SetLast();

	isPhaseChanging_ = true;
	fader_->SetFade(Fader::STATE::FADE_OUT);
}

void GameScene::UpdatePhase(void)
{
	/*phaseCnt_++;
	if (phaseCnt_ > PHASE_TIME)
	{
		fader_->SetFade(Fader::STATE::FADE_IN);
		phaseCnt_ = 0;
	}*/
}

void GameScene::DrawPhase(void)
{
	DrawString(0, 0, "フェーズ遷移中", 0xffffff, true);
}
void GameScene::LevelUpReflection()
{
	//プレイヤーごとに強化反映
	int plNum = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);
	for (int i = 0; i < plNum; i++)
	{
		if (level_->GetPreType(i) != LevelScreenManager::TYPE::MAX)
		{
			level_->EffectSyne(*playerMng_->GetPlayer(i), i);
		}
	}
	//ステート確認
	if (level_->GetState() != LevelScreenManager::STATE::NONE)
	{
		//通常時以外は処理しない
		return;
	}

	for (int i = 0; i < plNum; i++)
	{
		level_->Reflection(*playerMng_->GetPlayer(i), i);
	}
}

void GameScene::FazeResultUpdate(void)
{
	fazeResult_->Update();

	//リザルトが終了したとき
	if (fazeResult_->IsEnd())
	{
		//フェーズカウント増加
		fazeCnt_++;
		//カウント後最終フェーズ数より大きくなったらクリアシーンへ
		if (fazeCnt_ > LAST_FAZE)SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);


		//フェーズリザルトが終了したので明転及びリザルトリセット・タイマー初期化・BGMの再生
		fader_->SetFade(Fader::STATE::FADE_IN);
		Timer::GetInstance().Reset();
		fazeResult_->Reset();
		isFazeRezult_ = false;
		if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_LAST);
		else SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_NOMAL);
	}
}

void GameScene::InformFazeNum(void)
{
	informCnt_++;
}

bool GameScene::IsGameOver(void)
{
	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++) {
		PlayerBase* p = playerMng_->GetPlayer(i);

		if (p->IsAlive())return false;
	}
	return true;
}