#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/InputManager.h"
#include"../Object/Character/EnemySort/Enemy.h"
#include"../Object/Character/EnemyManager.h"
#include"../Object/Character/Chiken/ChickenManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelScreenManager.h"
#include "../Object/System/UnitPositionLoad.h"
#include "../Object/System/InformFaze.h"
#include "../Object/System/GameUi/PlayerUI.h"
#include "../Manager/GameSystem/DataBank.h"
#include"../Object/Character/PlayerDodge.h"
#include "../Object/System/GameUi/PlayerUI.h"
#include"FazeResult.h"
#include "GameScene.h"




GameScene::GameScene(void)
{
	stage_ = nullptr;

	sky_ = nullptr;

	fader_ = nullptr;
	isFadeInFinish_ = false;

	isPhaseChanging_ = false;
	phaseCnt_ = -1;

	fazeResult_ = nullptr;
	fazeCnt_ = -1;
	isInformFaze_ = false;
	inform_ = nullptr;
	isFazeRezult_ = false;

	playerMng_ = nullptr;
	enmMng_ = nullptr;

	level_ = nullptr;
	chicken_ = nullptr;
	unitLoad_ = nullptr;
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
	//UI
	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
	{
		uis_[i] = std::make_unique<PlayerUI>();
		uis_[i]->Init(*playerMng_->GetPlayer(i), DataBank::GetInstance().Output(i + 1));
	}


	//敵マネージャの生成
	enmMng_ = std::make_unique<EnemyManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::ENEMY));
	enmMng_->Init();
	//ニワトリの生成
	chicken_ = std::make_unique<ChickenManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::CPU),
		stage_->GetTtans(),
		playerMng_->GetPlayer(0)->GetTransform());
	chicken_->Init();

	//UI
	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
	{
		uis_[i] = std::make_unique<PlayerUI>();
		uis_[i]->Init(*playerMng_->GetPlayer(i), DataBank::GetInstance().Output(i + 1));
	}

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
	isFadeInFinish_ = true;

	//フェーズリザルトの作成
	fazeResult_ = std::make_unique<FazeResult>();
	fazeResult_->Init();

	//フェーズ数カウント
	fazeCnt_ = 0;
	inform_ = std::make_unique<InformFaze>();
	SetIsInform(true);


	//音声関係設定
	SoundInit();

	//エフェクト
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::HIT2, 
		ResourceManager::GetInstance().Load(ResourceManager::SRC::HIT2).handleId_);
	//プレイヤーアーチャーのバフ矢が当たった時のエフェクト
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::BUFF_ARROW_HIT,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::BUFF_ARROW_HIT).handleId_);

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
	//フェーズ数の知らせ(フェード終了後に更新をかける)
	if (isInformFaze_&& isFadeInFinish_) {
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

	//UI更新
	for (int i = 0; i < DataBank::GetInstance().Output(DataBank::INFO::USER_NUM); i++)
	{
		uis_[i]->Update(*playerMng_->GetPlayer(i));
	}

	//敵の更新
	enmMng_->Update();
	//敵の経験値取得処理
	if (enmMng_->GetAllExp() != 0.0f)
	{
		//経験値取得
		level_->AddExp(enmMng_->GetAllExp());
		//敵からの経験値初期化
		enmMng_->ClearAllExp();
	}

	
	//チキンの更新
	//chicken_->SetTargetPos(playerMng_->GetPlayer(0)->GetPos());
	chicken_->Update();

	for (int i = 0; i < DataBank::GetInstance().Output(DataBank::INFO::USER_NUM); i++)
	{
		uis_[i]->Update(*playerMng_->GetPlayer(i));
	}

	//あたり判定
	Collision();

	//強化要素の反映
	LevelUpReflection();



	//いずれ消す
	auto& ins = InputManager::GetInstance();
	auto& mng = SceneManager::GetInstance();
	////スペース推したらタイトルに戻る
	/*if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		mng.ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
	}*/

	/*if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		ChangePhase();
	}*/

	//if (ins.IsTrgDown(KEY_INPUT_K))
	//	playerMng_->GetPlayer(0)->SetDamage(20, 20);
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
	//ステージ
	stage_->Draw();
	////プレイヤー
	//playerMng_->Draw();

	//敵
	enmMng_->Draw();

	//チキン
	chicken_->Draw();

	//プレイヤー
	playerMng_->Draw();

	//レベル
	level_->Draw();

	//レベルアップ中タイマー及びUIは表示しない
	if (level_->IsLevelUp())return;
	
	//UIの描画
	uis_[SceneManager::GetInstance().GetNowWindow()]->Draw();

	//制限時間
	Timer::GetInstance().Draw();


	if (isInformFaze_) {
		inform_->Draw();
	}

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

	/*SceneManager::GetInstance().ResetCameras();
	SceneManager::GetInstance().ReturnSolo();*/
	Timer::GetInstance().Reset();
	Timer::GetInstance().Release();

	if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_LAST);
	else SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_NOMAL);

	EffectManager::GetInstance().Stop(EffectManager::EFFECT::HIT2);
	EffectManager::GetInstance().Stop(EffectManager::EFFECT::BUFF_ARROW_HIT);
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
	//音量設定
	snd.AdjustVolume(SoundManager::SOUND::GAME_LAST, 150);

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
	//衝突判定マネージャ取得
	auto& col = Collision::GetInstance();
	//敵の総数取得
	int maxCnt = enmMng_->GetActiveNum();

	enmMng_->CollisionStage(stage_->GetTtans());

	//あたり判定(主に索敵)
	for (int i = 0; i < maxCnt; i++)
	{
		//敵の取得
		Enemy* e = enmMng_->GetActiveEnemy(i);

		//敵個人の位置と攻撃を取得
		VECTOR ePos = e->GetPos();

		//敵個人の索敵判定
		bool isPlayerFound = false;

		for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
		{
			PlayerBase* p = playerMng_->GetPlayer(i);
			//プレイヤーが生きてなかったら処理を飛ばす
			if (!p->IsAlive())
				continue;
			VECTOR pPos = p->GetPos();

			//範囲内に入っているとき

			//通常状態時 && 攻撃範囲内にプレイヤーが入ったら攻撃を開始
			if (col.Search(ePos, pPos, e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//状態を変更
				e->ChangeState(Enemy::STATE::ALERT);
			}

			if (col.Search(ePos, pPos, e->GetSearchRange())) {
				//プレイヤーを狙う
				e->ChangeSearchState(Enemy::SEARCH_STATE::PLAYER_FOUND);
				e->SetTargetPos(pPos);

				//見つけた
				isPlayerFound = true;
			}
			else if (!isPlayerFound && e->GetSearchState() != Enemy::SEARCH_STATE::CHICKEN_FOUND) {
				//誰も狙っていない
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_SEARCH);
			}

			//攻撃判定
			for (int a = 0; a < e->GetAtks().size(); a++)
			{
				//攻撃情報をセット
				e->SetAtk(e->GetAtks()[a]);

				//セットしてきた情報をとってくる
				UnitBase::ATK eAtk = e->GetAtk();

				//アタック中 && 攻撃判定が終了していないときだけ処理する。それ以外はしないので戻る
				if (eAtk.IsAttack() && !eAtk.isHit_) {


					//攻撃が当たる範囲 && プレイヤーが回避していないとき
					if (col.IsHitAtk(*e, *p)/* && !p->GetDodge()->IsDodge()*/)
					{
						//ダメージ
						p->SetDamage(e->GetAtkPow(), eAtk.pow_);
						//使用した攻撃を判定終了に
						e->SetAtksIsHit(a, true);
					}
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

	//ステージとの判定
	playerMng_->CollisionStage(stage_->GetTtans());

	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
	{
		PlayerBase* p = playerMng_->GetPlayer(i);

		auto pPos = p->GetPos();
		auto pAtk = p->GetAtk();

		//変更箇所
		//--------------------------------
		auto pRole = p->GetRole();
		if (pRole == SceneManager::ROLE::ARCHER)
		{
			CollisionPlayerArrow(i);
			CollisionPlayerSerch(i);
		}
		//--------------------------------
		// 
		// 
		//プレイヤーがCPUの時だけサーチしたい
		//if (p->GetPlayMode() == SceneManager::PLAY_MODE::CPU)CollisionPlayerCPU(*p, pPos);

		//プレイヤー攻撃判定
		//攻撃していない || 攻撃がすでに当たっている
		if (!pAtk.IsAttack() || pAtk.isHit_)continue;

		for (int i = 0; i < maxCnt; i++)
		{
			//敵の取得
			Enemy* e = enmMng_->GetActiveEnemy(i);
			VECTOR ePos = e->GetPos();
			//当たり判定
			if (col.IsHitAtk(*p, *e)) {
				//被弾
				e->SetDamage(p->GetAtkPow(), p->GetAtk().pow_);
				EffectManager::GetInstance().Play(EffectManager::EFFECT::HIT2, ePos, Quaternion(), HIT_EFFECT_SIZE, SoundManager::SOUND::NONE);
				//攻撃判定の終了
				p->SetIsHit(true);
			}
		}
	}

}

void GameScene::CollisionPlayerArrow(int _p1Num)
{
	//衝突判定マネージャ取得
	auto& col = Collision::GetInstance();
	//敵の総数取得
	int maxCnt = enmMng_->GetActiveNum();



	PlayerBase* p = playerMng_->GetPlayer(_p1Num);

	auto pPos = p->GetPos();
	auto pAtk = p->GetAtk();

	//プレイヤーがCPUの時だけサーチしたい
	//if (p->GetPlayMode() == SceneManager::PLAY_MODE::CPU)CollisionPlayerCPU(*p, pPos);

	//プレイヤー攻撃判定
	//攻撃していない || 攻撃がすでに当たっている

	for (int enemy = 0; enemy < maxCnt; enemy++)
	{
		int pArrowCnt = p->GetArrowCnt(static_cast<int>(PlayerBase::ATK_TYPE::ATTACK));
		for (int arrowCnt = 0; arrowCnt < pArrowCnt; arrowCnt++)
		{
			if (p->GetAtks(PlayerBase::ATK_TYPE::ATTACK).empty())continue;
			auto arrow = p->GetArrowAtk(PlayerBase::ATK_TYPE::ATTACK, arrowCnt);

			if (!arrow.IsAttack() || arrow.isHit_)continue;
			p->SetAtk(arrow);
			//敵の取得
			Enemy* e = enmMng_->GetActiveEnemy(enemy);
			//当たり判定
			//if (col.IsHitArrowAtk(p, *e, arrowCnt)) {
			if (col.IsHitAtk(*p, *e)) {
				//被弾
				e->SetDamage(p->GetAtkPow(), p->GetAtk().pow_);
				EffectManager::GetInstance().Play(EffectManager::EFFECT::HIT2, e->GetPos(), Quaternion(), HIT_EFFECT_SIZE, SoundManager::SOUND::NONE);
				//攻撃判定の終了
				p->SetIsArrowHit(PlayerBase::ATK_TYPE::ATTACK, true, arrowCnt);
			}
		}

	}


	//アーチャーのバフ矢が当たったらバフをかける
	for (int pl = 0; pl < PlayerManager::PLAYER_NUM; pl++)
	{
		//当たり判定する者が自分自身だった場合無視する
		if (_p1Num == pl)continue;
		PlayerBase* p2 = playerMng_->GetPlayer(pl);


		int pArrowCnt = p->GetArrowCnt(static_cast<int>(PlayerBase::ATK_TYPE::BUFF));
		for (int arrowCnt = 0; arrowCnt < pArrowCnt; arrowCnt++)
		{
			auto arrow = p->GetArrowAtk(PlayerBase::ATK_TYPE::BUFF, arrowCnt);
			if (!arrow.IsAttack() || arrow.isHit_)continue;
			p->SetAtk(arrow);
			if (col.IsHitAtk(*p, *p2) && !p2->GetIsBuff(PlayerBase::SKILL_BUFF::BUFF_ARROW))
			{
				//アーチャーの弓が当たったら当たったプレイヤーの能力を上げる
				p->Buff(*p2);
				p2->SetIsBuff(PlayerBase::SKILL_BUFF::BUFF_ARROW, true);
				EffectManager::GetInstance().Play(
					EffectManager::EFFECT::BUFF_ARROW_HIT,
					p2->GetPos(),
					Quaternion(), 
					BUFF_ARROW_EFFECT_SIZE,
					SoundManager::SOUND::NONE);
				//攻撃判定の終了
				p->SetIsArrowHit(PlayerBase::ATK_TYPE::BUFF, true, arrowCnt);

			}
		}
	}

}

void GameScene::CollisionPlayerSerch(int _p1Num)
{
	//一番近い敵を探す
	//敵の数を取得
	int enmCnt = enmMng_->GetActiveNum();
	float e2pVecSize[EnemyManager::ENEMY_MAX];
	VECTOR minE2PVec;
	float min = FLT_MAX;
	auto& col = Collision::GetInstance();

		VECTOR pPos = playerMng_->GetPlayer(_p1Num)->GetPos();
		for (int ecnt = 0; ecnt < enmCnt; ecnt++)
		{
			VECTOR ePos1 = enmMng_->GetActiveEnemy(ecnt)->GetPos();

			//プレイヤーと敵との距離を求める
			float e2p = sqrt((ePos1.x - pPos.x) * (ePos1.x - pPos.x) + (ePos1.z - pPos.z) * (ePos1.z - pPos.z));
			e2pVecSize[ecnt] = e2p;
			if (e2pVecSize[ecnt] < min)
			{
				min = e2pVecSize[ecnt];
				minE2PVec = ePos1;
				playerMng_->GetPlayer(_p1Num)->SetTargetPos(minE2PVec);
				if (col.Search(pPos, ePos1, PlayerBase::ARCHER_SEARCH_RANGE))
				{
					playerMng_->GetPlayer(_p1Num)->SetIsSerchArcher(true);
				}
				else
				{
					playerMng_->GetPlayer(_p1Num)->SetIsSerchArcher(false);
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

			//索敵
			//範囲内に入っているとき
			if (col.Search(ePos, c->GetPos(), e->GetSearchRange())) {
				//移動を開始

				//鶏を狙う
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_FOUND);
				e->SetTargetPos(c->GetPos());
			}
			else if (e->GetSearchState() != Enemy::SEARCH_STATE::PLAYER_FOUND) {
				//移動を開始

				//まだ探し中
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_SEARCH);
				e->SetPreTargetPos(c->GetPos());
			}

			//通常状態時 && 攻撃範囲内にプレイヤーが入ったら攻撃を開始
			if (col.Search(ePos, c->GetPos(), e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//状態を変更
				e->ChangeState(Enemy::STATE::ALERT);
			}

			//攻撃判定
			for (int a = 0; a < e->GetAtks().size(); a++)
			{
				//攻撃情報をセット
				e->SetAtk(e->GetAtks()[a]);

				//セットしてきた情報をとってくる
				UnitBase::ATK eAtk = e->GetAtk();

				//アタック中 && 攻撃判定が終了していないときだけ処理する。それ以外はしないので戻る
				if (!(eAtk.IsAttack() && !eAtk.isHit_))continue;

				//攻撃が当たる範囲 && プレイヤーが回避していないとき
				if (col.IsHitAtk(*e, *c))
				{
					//ダメージ
					c->SetDamage(e->GetAtkPow(),e->GetAtk().pow_);
					//使用した攻撃を判定終了に
					e->SetAtksIsHit(a, true);
				}
			}
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
			isFadeInFinish_ = true;
			isPhaseChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())	//暗転終了
		{
			
			isFadeInFinish_ = false;
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

	data.Input(DataBank::INFO::FAZE_DUNK_ENEMY, enmMng_->GetDunkCnt());	//倒した敵数
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
		//カウント後最終フェーズ数より大きくなったらクリアシーンへ
		if (fazeCnt_ >= LAST_FAZE) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
		}

		level_->AddExp(fazeResult_->GetExp());
		

		//フェーズリザルトが終了したので明転及びリザルトリセット・タイマー初期化・BGMの再生
		fader_->SetFade(Fader::STATE::FADE_IN);
		Timer::GetInstance().Reset();
		fazeResult_->Reset();
		isFazeRezult_ = false;
		//通知機能をONに
		SetIsInform(true);
		
		//敵の入れ替え
		enmMng_->ProcessChangePhase(fazeCnt_);

		//プレイヤーのリセット
		playerMng_->ResetFaze();

		if (fazeCnt_ <= LAST_FAZE)
		{
			if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_LAST);
			else SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_NOMAL);
		}
	}
}

void GameScene::InformFazeNum(void)
{
	//知らせ更新
	if (!inform_->Update()) {
		SetIsInform(false);
	}

	//知らせ関係の描画処理やる
}

void GameScene::SetIsInform(const bool _flag)
{
	isInformFaze_ = _flag;
	if (_flag == true)
	{
		fazeCnt_++;		//true=フェーズ数更新時のためカウントを増加
		inform_->SetFazeCnt(fazeCnt_);
	}
}

bool GameScene::IsGameOver(void)
{
	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++) {
		PlayerBase* p = playerMng_->GetPlayer(i);

		if (p->IsAlive())return false;
	}
	return true;
}