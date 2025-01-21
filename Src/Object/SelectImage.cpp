#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/DataBank.h"
#include "../Scene/SelectScene.h"
#include "../Object/Character/SelectPlayer.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select, std::shared_ptr<SelectPlayer> player) : selectScene_(select), player_(player)
{
	imgPlayerNum_ = nullptr;
	imgLeftPoint_ = nullptr;
	imgRightPoint_ = nullptr;
	imgReady_ = nullptr;
	imgRoleNum_ = nullptr;

	state_ = SelectScene::SELECT::NUMBER;

	//座標(四角形を作るために2つの三角形を使う)
	mesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;		// 左下
	mesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;		// 右下
	mesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;		// 左上
	mesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;		// 右上

	readyMesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;		// 左下
	readyMesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;		// 右下
	readyMesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;		// 左上
	readyMesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;		// 右上

	pointL_.mesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;	// 左下
	pointL_.mesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;	// 右下
	pointL_.mesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;	// 左上
	pointL_.mesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;	// 右上

	pointR_.mesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;	// 左下
	pointR_.mesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;	// 右下
	pointR_.mesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;	// 左上
	pointR_.mesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;	// 右上

	//UV座標（テクスチャ座標）
	mesh_.vertex_[0].u = 0.0f / 4.0f;	mesh_.vertex_[0].v = 1.0f;	// 左下
	mesh_.vertex_[1].u = 1.0f / 4.0f;	mesh_.vertex_[1].v = 1.0f;	// 右下
	mesh_.vertex_[2].u = 0.0f / 4.0f;	mesh_.vertex_[2].v = 0.0f;	// 左上
	mesh_.vertex_[3].u = 1.0f / 4.0f;	mesh_.vertex_[3].v = 0.0f;	// 右上

	readyMesh_.vertex_[1].u = 1.0f;	readyMesh_.vertex_[1].v = 1.0f;	// 左下
	readyMesh_.vertex_[0].u = 0.0f;	readyMesh_.vertex_[0].v = 1.0f;	// 右下
	readyMesh_.vertex_[2].u = 0.0f;	readyMesh_.vertex_[2].v = 0.0f;	// 左上
	readyMesh_.vertex_[3].u = 1.0f;	readyMesh_.vertex_[3].v = 0.0f;	// 右上

	pointL_.mesh_.vertex_[0].u = 0.0f;	pointL_.mesh_.vertex_[0].v = 1.0f;	// 左下
	pointL_.mesh_.vertex_[1].u = 1.0f;	pointL_.mesh_.vertex_[1].v = 1.0f;	// 右下
	pointL_.mesh_.vertex_[2].u = 0.0f;	pointL_.mesh_.vertex_[2].v = 0.0f;	// 左上
	pointL_.mesh_.vertex_[3].u = 1.0f;	pointL_.mesh_.vertex_[3].v = 0.0f;	// 右上

	pointR_.mesh_.vertex_[0].u = 0.0f;	pointR_.mesh_.vertex_[0].v = 1.0f;	// 左下
	pointR_.mesh_.vertex_[1].u = 1.0f;	pointR_.mesh_.vertex_[1].v = 1.0f;	// 右下
	pointR_.mesh_.vertex_[2].u = 0.0f;	pointR_.mesh_.vertex_[2].v = 0.0f;	// 左上
	pointR_.mesh_.vertex_[3].u = 1.0f;	pointR_.mesh_.vertex_[3].v = 0.0f;	// 右上

	// 法線の設定（今回は省略、適当な値を設定）
	for (int i = 0; i < VERTEX_NUM; i++) {
		mesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);  //法線は-Z軸方向
		readyMesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);
		pointL_.mesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);
		pointR_.mesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);
	}

	// 色の設定（ディフューズカラー）
	for (int i = 0; i < VERTEX_NUM; i++) {
		mesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255);  //白色
		readyMesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255);
		pointL_.mesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255); 
		pointR_.mesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255); 
	}

	// 色の設定（ディフューズカラー）
	for (int i = 0; i < VERTEX_NUM; i++) {
		mesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);  //白色
		readyMesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);
		pointL_.mesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);
		pointR_.mesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);
	}

	leftTop_ = AsoUtility::VECTOR_ZERO;
	leftBottom_ = AsoUtility::VECTOR_ZERO;
	rightTop_ = AsoUtility::VECTOR_ZERO;
	rightBottom_ = AsoUtility::VECTOR_ZERO;

	playerNum_ = 1;
	isPad_ = false;
	role_ = 0;
	isReady_ = false;

	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
	press_ = false;
	angle_ = 0.0f;

	target_[0] = AsoUtility::VECTOR_ZERO;
	target_[1] = AsoUtility::VECTOR_ZERO;
	target_[2] = AsoUtility::VECTOR_ZERO;
	target_[3] = AsoUtility::VECTOR_ZERO;

	lerpTime_ = 0.0f;

	// 状態管理
	stateChanges_.emplace(
		SelectScene::SELECT::NUMBER, std::bind(&SelectImage::ChangeStateNumber, this));
	stateChanges_.emplace(
		SelectScene::SELECT::OPERATION, std::bind(&SelectImage::ChangeStateOperation, this));
	stateChanges_.emplace(
		SelectScene::SELECT::ROLE, std::bind(&SelectImage::ChangeStateRole, this));
}

void SelectImage::Destroy(void)
{
}

void SelectImage::Init(void)
{
	Load();

	InitVertex();

	target_[0] = SelectScene::DEFAULT_TARGET_POS;

	lerpTime_ = 1.0f;

	//人数選択から
	ChangeSelect(SelectScene::SELECT::NUMBER);
}

void SelectImage::Update(void)
{
	//更新ステップ
	stateUpdate_();
}

void SelectImage::Draw(void)
{
	switch (selectScene_.GetSelect())
	{
	case SelectScene::SELECT::NUMBER:
		NumberDraw();
		break;

	case SelectScene::SELECT::OPERATION:
		OperationDraw();
		break;

	case SelectScene::SELECT::ROLE:
		RoleDraw();
		break;

	default:
		break;
	}

#ifdef DRAW_DEBUG
	//DrawFormatString(Application::SCREEN_SIZE_X - 100, 0, 0x000000, "L : %d", pointL_.isToggle_);
	//DrawFormatString(Application::SCREEN_SIZE_X - 100, 20, 0x000000, "R : %d", pointR_.isToggle_);
	//DrawFormatString(Application::SCREEN_SIZE_X - 100, 40, 0x000000, "num : %d", playerNum_);
	//DrawFormatString(Application::SCREEN_SIZE_X - 100, 80, 0x000000, "pad : %d", isPad_);
	//for (int i = 0; i < 4; i++)
	//{
	//	DrawFormatString(Application::SCREEN_SIZE_X - 250, 100 + (20 * i), 0x000000,
	//		"L_mesh : %0.2f,%0.2f,%0.2f", pointL_.mesh_.vertex_[i].pos.x,
	//		pointL_.mesh_.vertex_[i].pos.y, pointL_.mesh_.vertex_[i].pos.z);
	//}
	//auto camera = SceneManager::GetInstance().GetCameras();
	//DrawFormatString(Application::SCREEN_SIZE_X - 100, 120, 0x000000, "camera : %d", camera.size());

	//球
	////左下
	//DrawSphere3D(pointL_.mesh_.vertex_[0].pos, 5.0f, 10, 0xFF0000, 0xFF0000, false);
	////右下
	//DrawSphere3D(pointL_.mesh_.vertex_[1].pos, 5.0f, 10, 0x00FF00, 0x00FF00, false);
	////左上
	//DrawSphere3D(pointL_.mesh_.vertex_[2].pos, 5.0f, 10, 0x0000FF, 0x0000FF, false);
	////右上
	//DrawSphere3D(pointL_.mesh_.vertex_[3].pos, 5.0f, 10, 0x000000, 0x000000, false);

#endif // DRAW_DEBUG
}

void SelectImage::MoveVertexPos(void)
{
	auto t = SceneManager::GetInstance().GetDeltaTime();

	lerpTime_ -= (t / 0.7f);
	if (lerpTime_ < 0.0f)
	{
		lerpTime_ = 0.0f;
		//return;
	}

	mesh_.vertex_[0].pos = { -70.0f, 60.0f, VERTEX_Z + 12.0f };		//左下
	mesh_.vertex_[1].pos = { 0.0f, 60.0f, VERTEX_Z + 12.0f };		//右下
	mesh_.vertex_[2].pos = { -70.0f, 170.0f, VERTEX_Z };			//左上
	mesh_.vertex_[3].pos = { 0.0f, 170.0f, VERTEX_Z };				//右上

	pointL_.mesh_.vertex_[0].pos = { POINT_LEFT_X - 40.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// 左下
	pointL_.mesh_.vertex_[1].pos = { POINT_RIGHT_X - 40.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// 右下
	pointL_.mesh_.vertex_[2].pos = { POINT_LEFT_X - 40.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// 左上
	pointL_.mesh_.vertex_[3].pos = { POINT_RIGHT_X - 40.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// 右上

	pointR_.mesh_.vertex_[0].pos = { -POINT_RIGHT_X + 40.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// 左下
	pointR_.mesh_.vertex_[1].pos = { -POINT_LEFT_X + 40.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// 右下
	pointR_.mesh_.vertex_[2].pos = { -POINT_RIGHT_X + 40.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// 左上
	pointR_.mesh_.vertex_[3].pos = { -POINT_LEFT_X + 40.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// 右上

}

void SelectImage::Load(void)
{
	//画像
	imgPlayerNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_NUM).handleIds_;

	imgRightPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RIGHT_POINT).handleIds_;

	imgLeftPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEFT_POINT).handleIds_;

	imgReady_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::READY).handleIds_;

	imgRoleNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLE).handleIds_;

	imgDeviceNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DEVICE).handleIds_;
}

void SelectImage::NumberUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//人数を１追加(中身は1～4に収める)
			playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1 : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//人数を１削除(中身は1～4に収める)
			playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
			if (playerNum_ == 0)playerNum_ = 4;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM : interval_;
			if (playerNum_ == 0)playerNum_ = 4;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下で決定&入力デバイス選択へ
	if (selectScene_.GetConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//プレイヤー人数の設定
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
		
		for (int i = 1; i <= playerNum_; i++)
		{
			//ディスプレイの設定
			data.Input(DataBank::INFO::DHISPLAY_NUM, i);
			data.Input(DataBank::INFO::USER_NUM, i);
		}

		for (int i = playerNum_ + 1; i <= SceneManager::PLAYER_NUM; i++)
		{
			//CPU人数の設定(CPUは１人から３人)
			data.Input(SceneManager::PLAY_MODE::CPU, i);
			data.Input(SceneManager::ROLE::KNIGHT, i);
		}

		//ウィンドウ複製の準備
		SceneManager::GetInstance().RedySubWindow();

		//カメラの設定
		auto cameras = SceneManager::GetInstance().GetCameras();
		for (int i = 1; i < cameras.size(); i++)
		{
			//最初の座標を保持
			VECTOR prevPos = target_[i - 1];
			//XZ平面で座標を回転させる
			target_[i] = AsoUtility::RotXZPos(SelectScene::DEFAULT_CAMERA_POS, prevPos, AsoUtility::Deg2RadF(90.0f));

			cameras[i]->SetPos(SelectScene::DEFAULT_CAMERA_POS, target_[i]);
			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
		}

		//if (playerNum_ > 1)
		//{
		//	//プレイヤー2以上の場合、2P以上のコントローラーをPAD操作に設定
		//	for (int num = 2; num <= playerNum_; num++)
		//	{
		//		data.Input(SceneManager::CNTL::PAD, num);
		//	}
		//}

		selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
		ChangeSelect(SelectScene::SELECT::OPERATION);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV座標（テクスチャ座標）
	mesh_.vertex_[0].u = ((float)(playerNum_)-1.0f) / 4.0f;		mesh_.vertex_[0].v = 1.0f;	// 左下
	mesh_.vertex_[1].u = (float)(playerNum_) / 4.0f;			mesh_.vertex_[1].v = 1.0f;	// 右下
	mesh_.vertex_[2].u = ((float)(playerNum_)-1.0f) / 4.0f;		mesh_.vertex_[2].v = 0.0f;	// 左上
	mesh_.vertex_[3].u = (float)(playerNum_) / 4.0f;			mesh_.vertex_[3].v = 0.0f;	// 右上

	pointL_.mesh_.vertex_[0].u = 0.0f;	pointL_.mesh_.vertex_[0].v = 1.0f;	// 左下
	pointL_.mesh_.vertex_[1].u = 1.0f;	pointL_.mesh_.vertex_[1].v = 1.0f;	// 右下
	pointL_.mesh_.vertex_[2].u = 0.0f;	pointL_.mesh_.vertex_[2].v = 0.0f;	// 左上
	pointL_.mesh_.vertex_[3].u = 1.0f;	pointL_.mesh_.vertex_[3].v = 0.0f;	// 右上
}

void SelectImage::OperationUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//キーを押した際に選択しているものを反転(2種類しかないので)
			isPad_ = !isPad_;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = !isPad_ : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//キーを押した際に選択しているものを反転(2種類しかないので)
			isPad_ = !isPad_;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = !isPad_ : interval_;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下で決定&役職選択へ
	if (selectScene_.GetConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//1Pの操作の設定
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		(isPad_) ? selectScene_.Set1PDevice(SceneManager::CNTL::PAD) : selectScene_.Set1PDevice(SceneManager::CNTL::KEYBOARD);
		//selectedCntl_ = (isPad_) ? SceneManager::CNTL::PAD : SceneManager::CNTL::KEYBOARD;

		selectScene_.ChangeSelect(SelectScene::SELECT::ROLE);
		ChangeSelect(SelectScene::SELECT::ROLE);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV座標（テクスチャ座標）
	mesh_.vertex_[0].u = (float)(isPad_) / 4.0f;			mesh_.vertex_[0].v = 1.0f;	// 左下
	mesh_.vertex_[1].u = ((float)(isPad_)+1.0f) / 4.0f;		mesh_.vertex_[1].v = 1.0f;	// 右下
	mesh_.vertex_[2].u = (float)(isPad_) / 4.0f;			mesh_.vertex_[2].v = 0.0f;	// 左上
	mesh_.vertex_[3].u = ((float)(isPad_)+1.0f) / 4.0f;		mesh_.vertex_[3].v = 0.0f;	// 右上

}

void SelectImage::RoleUpdate(void)
{
}

void SelectImage::NumberDraw(void)
{

	mesh_.DrawTwoMesh(*imgPlayerNum_);

	PointsDraw();

}

void SelectImage::OperationDraw(void)
{
	mesh_.DrawTwoMesh(*imgDeviceNum_);

	PointsDraw();
}

void SelectImage::RoleDraw(void)
{
	if (GetReady() != true)
	{
		mesh_.DrawTwoMesh(*imgRoleNum_);
	}
	else
	{
		mesh_.DrawTwoMesh(*imgRoleNum_);
		SetDrawBlendMode(DX_BLENDMODE_SUB, BLEND_PARAM);
		mesh_.DrawTwoMesh(*imgRoleNum_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドモードを戻す

		readyMesh_.DrawTwoMesh(*imgReady_);
	}

	PointsDraw();
}

void SelectImage::PointsDraw(void)
{
	//左の矢印の描画(右の矢印は暗めにする)
	if (pointL_.isToggle_)
	{
		////右の矢印画像を描画し、同じ画像を減算ブレンドする
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
		SetDrawBlendMode(DX_BLENDMODE_SUB, BLEND_PARAM);
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドモードを戻す

		//普通に描画
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
	}
	//右の矢印の描画(左の矢印は暗めにする)
	if (pointR_.isToggle_)
	{
		//左の矢印画像を描画し、同じ画像を減算ブレンドする
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
		SetDrawBlendMode(DX_BLENDMODE_SUB, BLEND_PARAM);
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドモードを戻す

		//普通に描画
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
	}

	//どちらの矢印も選んでいないときの描画
	if (!pointL_.isToggle_ && !pointR_.isToggle_)
	{
		//普通に描画
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
		//↑の上の画像に減算ブレンドする
		SetDrawBlendMode(DX_BLENDMODE_SUB, BLEND_PARAM);
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void SelectImage::ChangeObject(SelectScene::Device& input, int i)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	if (isReady_ == true)
	{
		if (input.config_ == SelectScene::KEY_CONFIG::CANCEL)
		{
			isReady_ = false;
		}
		return;
	}

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//役職を選択
			role_ = (role_ + 1) % SceneManager::PLAYER_NUM;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ + 1) % SceneManager::PLAYER_NUM : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	if (pointL_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//役職を選択
			role_ = (role_ - 1 + SceneManager::PLAYER_NUM) % SceneManager::PLAYER_NUM;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ - 1 + SceneManager::PLAYER_NUM) % SceneManager::PLAYER_NUM : interval_;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下でゲーム画面へ
	if (input.config_ == SelectScene::KEY_CONFIG::DECIDE)
	{
		//役職の設定
		data.Input(static_cast<SceneManager::ROLE>(role_ + 1), i + 1);

		isReady_ = true;
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV座標（テクスチャ座標）
	mesh_.vertex_[0].u = (float)(role_) / 4.0f;			mesh_.vertex_[0].v = 1.0f;	// 左下
	mesh_.vertex_[1].u = ((float)(role_)+1.0f) / 4.0f;	mesh_.vertex_[1].v = 1.0f;	// 右下
	mesh_.vertex_[2].u = (float)(role_) / 4.0f;			mesh_.vertex_[2].v = 0.0f;	// 左上
	mesh_.vertex_[3].u = ((float)(role_)+1.0f) / 4.0f;	mesh_.vertex_[3].v = 0.0f;	// 右上
}

void SelectImage::ChangeSelect(const SelectScene::SELECT _state)
{
	//状態遷移
	state_ = _state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void SelectImage::InitVertex(void)
{
	leftTop_ = { VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z };
	leftBottom_ = { VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_UNDER_Z };
	rightTop_ = { VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z };
	rightBottom_ = { VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_UNDER_Z };

	//座標(四角形を作るために2つの三角形を使う)
	mesh_.vertex_[0].pos = leftBottom_;		// 左下
	mesh_.vertex_[1].pos = rightBottom_;	// 右下
	mesh_.vertex_[2].pos = leftTop_;		// 左上
	mesh_.vertex_[3].pos = rightTop_;		// 右上

	readyMesh_.vertex_[0].pos = { -80.0f, 60.0f, VERTEX_UNDER_Z };	// 左下
	readyMesh_.vertex_[1].pos = { 80.0f, 60.0f, VERTEX_UNDER_Z };	// 右下
	readyMesh_.vertex_[2].pos = { -80.0f, 150.0f, VERTEX_Z };		// 左上
	readyMesh_.vertex_[3].pos = { 80.0f, 150.0f, VERTEX_Z };		// 右上

	pointL_.mesh_.vertex_[0].pos = { POINT_LEFT_X, POINT_UNDER_Y, POINT_TOP_Z };	// 左下
	pointL_.mesh_.vertex_[1].pos = { POINT_RIGHT_X, POINT_UNDER_Y, POINT_TOP_Z };	// 右下
	pointL_.mesh_.vertex_[2].pos = { POINT_LEFT_X, POINT_TOP_Y, POINT_UNDER_Z };	// 左上
	pointL_.mesh_.vertex_[3].pos = { POINT_RIGHT_X, POINT_TOP_Y, POINT_UNDER_Z };	// 右上

	pointR_.mesh_.vertex_[0].pos = { -POINT_RIGHT_X, POINT_UNDER_Y, POINT_TOP_Z };	// 左下
	pointR_.mesh_.vertex_[1].pos = { -POINT_LEFT_X, POINT_UNDER_Y, POINT_TOP_Z };	// 右下
	pointR_.mesh_.vertex_[2].pos = { -POINT_RIGHT_X, POINT_TOP_Y, POINT_UNDER_Z };	// 左上
	pointR_.mesh_.vertex_[3].pos = { -POINT_LEFT_X, POINT_TOP_Y, POINT_UNDER_Z };	// 右上

}

VECTOR SelectImage::RotateVertex(VECTOR pos, VECTOR center, float angle)
{
	//メッシュの中心点を計算(今はとりあえずメッシュの真ん中の座標)
	VECTOR Center;
	Center.x = (VERTEX_RIGHT_X + VERTEX_LEFT_X) * 0.5f;
	Center.y = (VERTEX_TOP_Y + VERTEX_UNDER_Y) * 0.5f;
	Center.z = VERTEX_Z;

	//半径
	const float DIS = 50.0f;

	// 時間進行
	angle_ += SceneManager::GetInstance().GetDeltaTime();

	// 回転行列を適用して頂点を回転

	//for (int i = 1; i < 4; i += 2)
	//{
		//XZ平面上で回転させる
	//	vertices_[i].pos.x = Center.x + (DIS * (sinf(angle_)));
	//	vertices_[i].pos.z = Center.z + (DIS * (cosf(angle_)));
	//}
	//for (int i = 0; i < 4; i += 2)
	//{
	//	vertices_[i].pos.x = Center.x + (-DIS * (sinf(angle_)));
	//	vertices_[i].pos.z = Center.z + (-DIS * (cosf(angle_)));
	//}
	return AsoUtility::VECTOR_ZERO;
}

void SelectImage::ChangeStateNumber(void)
{
	stateUpdate_ = std::bind(&SelectImage::NumberUpdate, this);
}

void SelectImage::ChangeStateOperation(void)
{
	stateUpdate_ = std::bind(&SelectImage::OperationUpdate, this);
}

void SelectImage::ChangeStateRole(void)
{
	MoveVertexPos();
	stateUpdate_ = std::bind(&SelectImage::RoleUpdate, this);
}

void SelectImage::Mesh::DrawTwoMesh(int handle)
{
	// 2つの三角形を描画（テクスチャ付き）
	// 1つ目の三角形
	VERTEX3D triangle1[3] = { vertex_[2], vertex_[1], vertex_[0] };
	// 2つ目の三角形
	VERTEX3D triangle2[3] = { vertex_[1], vertex_[2], vertex_[3] };

	// 三角形を描画
	DrawPolygon3D(triangle1, 1, handle, true);  // 1つ目の三角形
	DrawPolygon3D(triangle2, 1, handle, true);  // 2つ目の三角形
}
