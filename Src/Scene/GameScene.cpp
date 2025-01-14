#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/Timer.h"
#include "../Object/Character/PlayerBase.h"
#include "../Object/Character/PlayableChara/PlAxeMan.h"
#include "../Object/Character/PlayableChara/PlArcher.h"
#include"../Object/Character/Enemy.h"
#include"../Object/Character/EnemyManager.h"
#include"../Object/Character/Chiken/ChickenManager.h"
#include"../Object/Character/EnemySort/EneAxe.h"
#include"../Object/Character/EnemySort/EneGolem.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelScreenManager.h"
#include "../Object/System/UnitPositionLoad.h"
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

	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	sky_ = std::make_unique<SkyDome>();
	sky_->Init();

	level_ = std::make_unique<LevelScreenManager>();
	level_->Init();

	chicken_ = std::make_unique<ChickenManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::CPU));
	chicken_->Init();

	//プレイヤー設定
	for (int i = 0; i < PLAYER_NUM; i++)
	{
		players_[i] = std::make_unique<PlArcher>(SceneManager::PLAY_MODE::USER, InputManager::JOYPAD_NO::PAD1);
		players_[i]->Init();
	}

	//敵マネージャの生成
	enmMng_ = std::make_unique<EnemyManager>();
	enmMng_->Init();

	std::unique_ptr<Enemy> e=std::make_unique<EneAxe>();
	e->Init();
	enemys_.push_back(std::move(e));

	std::unique_ptr<Enemy> g = std::make_unique<EneGolem>();
	g->Init();
	enemys_.push_back(std::move(g));

	//カメラの設定
	auto cameras = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < cameras.size(); i++)
	{
		cameras[i]->SetFollow(&players_[i]->GetTransform());
		cameras[i]->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	}

	//フェーダーの作成
	fader_ = std::make_unique<Fader>();
	fader_->Init();
	//フェーズリザルトの作成
	fazeResult_ = std::make_unique<FazeResult>();
	fazeResult_->Init();
}

void GameScene::Update(void)
{
	//ゲームオーバー判定
	if(IsGameOver())SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);

	//フェーズリザルト
	if (isFazeRezult_)
	{
		fazeResult_->Update();

		if (fazeResult_->IsEnd())
		{
			//フェーズリザルトが終了したので明転
			fader_->SetFade(Fader::STATE::FADE_IN);
			Timer::GetInstance().Reset();
			fazeResult_->Reset();
			isFazeRezult_ = false;
		}
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

	Timer::GetInstance().Update();
	//タイマーが終了したら
	if (Timer::GetInstance().IsEnd())ChangePhase();

	level_->Update();

	//プレイヤー�@だけを動かしています
	players_[0]->Update();


	//敵の更新
	enmMng_->Update();
	for (auto& e : enemys_)
	{
		e->SetTargetPos(players_[0]->GetPos());
		e->Update();
	}
	

	chicken_->SetTargetPos(players_[0]->GetPos());
	chicken_->Update();

	//あたり判定
	Collision();

	//強化要素の反映
	LevelUpReflection();



	//いずれ消す
	auto& ins = InputManager::GetInstance();
	auto& mng = SceneManager::GetInstance();
	//スペース推したらタイトルに戻る
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		mng.ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		ChangePhase();
	}
}

void GameScene::Draw(void)
{
	if (isFazeRezult_)
	{
		fazeResult_->Draw();
		return;
	}

	sky_->Draw();

	for (auto& p : players_)
		p->Draw();

	for (auto& e : enemys_){
		e->Draw();
	}
	enmMng_->Draw();

	stage_->Draw();
	chicken_->Draw();
	level_->Draw();

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
	stage_->Release();

	SceneManager::GetInstance().ResetCameras();
	SceneManager::GetInstance().ReturnSolo();

	for (auto& p : players_)
	{
		p->Destroy();
	}

	for (auto& e : enemys_)
	{
		e->Destroy();
	}
	enmMng_->Release();
}


//当たり判定（他項目に干渉するもののみ）
//あたり判定総括
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();

	CollisionEnemy();
	CollisionPlayer();

}

//敵関係の当たり判定
void GameScene::CollisionEnemy(void)
{
	auto& col = Collision::GetInstance();

	//あたり判定(主に索敵)
	for (auto& e : enemys_)
	{
		//敵個人の位置と攻撃を取得
		VECTOR ePos = e->GetPos();
		UnitBase::ATK eAtk = e->GetAtk();

		//動ける分だけ(のちに全員分に変える)
		VECTOR pPos = players_[0]->GetPos();

		//索敵
		//範囲内に入っているとき
		if (col.Search(ePos, pPos, e->GetSearchRange())) {
			//移動を開始
			e->SetIsMove(true);
		}else {
			//移動を停止
			e->SetIsMove(false);
		}

		//通常状態時 && 攻撃範囲内にプレイヤーが入ったら攻撃を開始
		if (col.Search(ePos, pPos, e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
			//状態を変更
			e->ChangeState(Enemy::STATE::ALERT);
		}

		//攻撃判定
		//アタック中 && 攻撃判定が終了していないとき
		if (eAtk.IsAttack() && !eAtk.isHit_)
		{
			//各プレイヤーと当たり判定を取る
			for (auto& p : players_)
			{
				//攻撃が当たる範囲 && プレイヤーが回避していないとき
				if (col.IsHitAtk(*e, *p) && !p->IsDodge())
				{
					//ダメージ
					p->Damage();
					//使用した攻撃を判定終了に
					e->SetIsHit(true);
				}
			}
		}
	}
}

void GameScene::CollisionPlayer(void)
{
	auto& col = Collision::GetInstance();

	for (auto& p : players_)
	{
		auto pPos = p->GetPos();
		auto pAtk = p->GetAtk();

		//プレイヤーがCPUの時だけサーチしたい
		if (p->GetPlayMode() == SceneManager::PLAY_MODE::CPU)CollisionPlayerCPU(*p, pPos);

		//プレイヤー攻撃判定
		//攻撃していない || 攻撃がすでに当たっている
		if (!pAtk.IsAttack() || pAtk.isHit_)continue;

		for (auto& e : enemys_)
		{
			//当たり判定
			if (col.IsHitAtk(*p, *e)) {
				//被弾
				e->Damage(5, 4);
				//攻撃判定の終了
				p->SetIsHit(true);
			}
		}
		
	}
}

void GameScene::CollisionPlayerCPU(PlayerBase& _player, const VECTOR& _pPos)
{
	auto& col = Collision::GetInstance();

	//敵をサーチ初期化
	_player.SetisEnemySerch(false);

	//敵の個体分行う
	for (auto& e : enemys_)
	{
		//敵が死亡していたら処理しない
		if (!e->IsAlive())continue;

		//敵個人の位置と攻撃を取得
		VECTOR ePos = e->GetPos();

		//プレイヤー側索敵
		if (col.Search(_pPos, ePos, _player.GetSearchRange())
			&& !_player.GetIsCalledPlayer())
		{
			//敵をサーチしたかを返す
			_player.SetisEnemySerch(true);
			_player.SetTargetPos(ePos);
		}

		if (col.Search(_player.GetPos(), ePos, _player.GetAtkStartRange())
			&& _player.GetState() == PlayerBase::CPU_STATE::NORMAL
			&& !_player.GetIsCalledPlayer())
		{
			//状態を変更
			_player.ChangeState(PlayerBase::CPU_STATE::ATTACK);
		}
	}
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
	//ステート確認
	if (level_->GetState() == LevelScreenManager::STATE::FIN)
	{
		return;
	}

	//ここでプレイヤーの強化を反映
	switch (level_->GetType())
	{
	case LevelScreenManager::TYPE::ATTACK:
		break;

	case LevelScreenManager::TYPE::DEFENSE:
		break;

	case LevelScreenManager::TYPE::LIFE:
		break;

	case LevelScreenManager::TYPE::SPEED:
		break;

	default:
		break;
	}
}

bool GameScene::IsGameOver(void)
{
	for (auto& p : players_) {
		if (p->IsAlive())return false;
	}
	return true;
}
