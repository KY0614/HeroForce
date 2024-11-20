#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"
#include "../Object/Character/PlayerBase.h"
#include "../Object/Character/PlayableChara/AxeMan.h"
#include"../Object/Character/Enemy.h"
#include"../Object/Character/EnemySort/EneAxe.h"
#include"../Object/Character/EnemySort/EneArcher.h"
#include "../Object/Common/Transform.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	

	// グリッド線機能の実態を生成
	grid_ = new Grid();
	grid_->Init();	

#ifdef _DEBUG_COL
	playerTest_ = new PlAxe(SceneManager::PLAY_MODE::USER);
	playerTest_->Init();
	playerTest_->ChangeControll(SceneManager::CNTL::KEYBOARD);
	enemyTest_ = new EneArcher();
	enemyTest_->Init();
#endif

	// カメラモード　：フリーカメラ
	Camera* camera = SceneManager::GetInstance().GetCamera();
	//カメラの対象設定
	//camera->SetFollow(&playerShip_->GetTransform());
	camera->ChangeMode(Camera::MODE::FREE);
}

void GameScene::Update(void)
{
	grid_->Update();
}

void GameScene::Draw(void)
{
	grid_->Draw();
}

void GameScene::Release(void)
{
	delete grid_;
#ifdef _DEBUG_COL
	playerTest_->Destroy();
	delete playerTest_;
	enemyTest_->Destroy();
	delete enemyTest_;
#endif
}


//当たり判定（他項目に干渉するもののみ）
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();


#ifdef _DEBUG_COL

	auto ePos = enemyTest_->GetPos();
	auto eAtk = enemyTest_->GetAtk();
	auto pPos = playerTest_->GetPos();
	auto pAtk = playerTest_->GetAtk();

	//敵側索敵
	if (col.Search(ePos, pPos, enemyTest_->GetSearchRange()))
	{
		//移動を開始
		enemyTest_->SetIsMove(true);
	}
	else
	{
		//移動を停止
		enemyTest_->SetIsMove(false);
	}


	if (col.Search(ePos, pPos, enemyTest_->GetAtkStartRange()) 
		&& enemyTest_->GetState() == Enemy::STATE::NORMAL)
	{
		//状態を変更
		enemyTest_->ChangeState(Enemy::STATE::ALERT);
	}


	//プレイヤー側索敵
	if (col.Search(playerTest_->GetPos(), enemyTest_->GetPos(), playerTest_->GetAtkStartRange()))
	{
		//状態を変更
		playerTest_->ChangeState(PlayerBase::STATE::ATTACK);
	}

	//プレイヤー攻撃判定
	//攻撃中でありその攻撃が一度も当たっていないか
	if (pAtk.IsAttack()&&!pAtk.isHit_)
	{
		//当たり判定
		if (col.IsHitAtk(playerTest_, enemyTest_))	
		{
			//被弾
			enemyTest_->Damage(5, 4);				
			//攻撃判定の終了
			playerTest_->SetIsHit(true);			
		}
	}

	//敵の攻撃判定
	//アタック中であり攻撃判定が終了していないとき
	if (eAtk.IsAttack() && !eAtk.isHit_)
	{
		//攻撃が当たる範囲であり、プレイヤーが回避していないとき
		if (col.IsHitAtk(enemyTest_, playerTest_) && !playerTest_->IsDodge())
		{
			//ダメージ
			playerTest_->Damage();
			//使用した攻撃を判定終了に
			enemyTest_->SetIsHit(true);
		}
	}

#endif


}
