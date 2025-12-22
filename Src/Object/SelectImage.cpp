#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Scene/SelectScene.h"
#include "../Object/Character/PlayableChara/Other/SelectCharacter.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select) : selectScene_(select)
{
	imgPlayerNum_ = nullptr;
	imgDisplayNum_ = nullptr;
	imgLeftPoint_ = nullptr;
	imgRightPoint_ = nullptr;
	imgReady_ = nullptr;
	imgRoleNum_ = nullptr;
	imgDeviceNum_ = nullptr;
	imgComingSoon_ = nullptr;

	state_ = SelectScene::SELECT::NUMBER;

	//座標(四角形を作るために2つの三角形を使う)
	mesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;		// 左下
	mesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;		// 右下
	mesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;		// 左上
	mesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;		// 右上

	readyMesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;	// 左下
	readyMesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;	// 右下
	readyMesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;	// 左上
	readyMesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;	// 右上

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

	displayNum_ = -1;
	playerNum_ = -1;
	isPad_ = false;
	role_ = -1;
	isReady_ = false;

	keyPressTime_ = -1.0f;
	addIntervalTime_ = -1.0f;
	isPress_ = false;
	angle_ = -1.0f;

	target_[0] = AsoUtility::VECTOR_ZERO;
	target_[1] = AsoUtility::VECTOR_ZERO;
	target_[2] = AsoUtility::VECTOR_ZERO;
	target_[3] = AsoUtility::VECTOR_ZERO;

	// 状態管理
	stateChanges_.emplace(
		SelectScene::SELECT::DISPLAY, std::bind(&SelectImage::ChangeStateDisplay, this));
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
	LoadImages();
	LoadSounds();

	InitVertex();

	displayNum_ = 1;
	playerNum_ = 1;
	isPad_ = false;
	role_ = 1;
	isReady_ = false;
	keyPressTime_ = 0.0f;
	addIntervalTime_ = 0.0f;
	isPress_ = false;
	angle_ = 0.0f;

	target_[0] = SelectScene::DEFAULT_TARGET_POS;

	//ディスプレイ数選択から
	ChangeSelect(SelectScene::SELECT::DISPLAY);
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
	case SelectScene::SELECT::DISPLAY:
		DisplayDraw();
		break;

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

}

void SelectImage::MoveVertexPos(void)
{
	mesh_.vertex_[0].pos = { ROLE_MESH_LEFT_X, ROLE_MESH_UNDER_Z, VERTEX_Z + 12.0f };	//左下
	mesh_.vertex_[1].pos = { ROLE_MESH_RIGHT_X, ROLE_MESH_UNDER_Z, VERTEX_Z + 12.0f };	//右下
	mesh_.vertex_[2].pos = { ROLE_MESH_LEFT_X, ROLE_MESH_TOP_Z, VERTEX_Z };				//左上
	mesh_.vertex_[3].pos = { ROLE_MESH_RIGHT_X, ROLE_MESH_TOP_Z, VERTEX_Z };			//右上

	pointL_.mesh_.vertex_[0].pos = { POINT_LEFT_X - 25.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// 左下
	pointL_.mesh_.vertex_[1].pos = { POINT_RIGHT_X - 25.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// 右下
	pointL_.mesh_.vertex_[2].pos = { POINT_LEFT_X - 25.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// 左上
	pointL_.mesh_.vertex_[3].pos = { POINT_RIGHT_X - 25.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// 右上

	pointR_.mesh_.vertex_[0].pos = { -POINT_RIGHT_X + 25.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// 左下
	pointR_.mesh_.vertex_[1].pos = { -POINT_LEFT_X + 25.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// 右下
	pointR_.mesh_.vertex_[2].pos = { -POINT_RIGHT_X + 25.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// 左上
	pointR_.mesh_.vertex_[3].pos = { -POINT_LEFT_X + 25.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// 右上

	readyMesh_.vertex_[0].pos = { -70.0f, 70.0f, -408.0f };	// 左下
	readyMesh_.vertex_[1].pos = { 90.0f, 70.0f, -408.0f };	// 右下
	readyMesh_.vertex_[2].pos = { -70.0f, 160.0f, -410.0f };// 左上
	readyMesh_.vertex_[3].pos = { 90.0f, 160.0f, -410.0f };	// 右上
}

void SelectImage::ReductionVertexPos(void)
{
	readyMesh_.vertex_[0].pos = { -60.0f, 80.0f, -408.0f };	// 左下
	readyMesh_.vertex_[1].pos = { 60.0f, 80.0f, -408.0f };	// 右下
	readyMesh_.vertex_[2].pos = { -60.0f, 150.0f, -410.0f };// 左上
	readyMesh_.vertex_[3].pos = { 60.0f, 150.0f, -410.0f };	// 右上
}

void SelectImage::DisplayUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();
	auto& snd = SoundManager::GetInstance();
	bool sound = false;

	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!isPress_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			isPress_ = true;

			//人数を１追加(中身は1～4に収める)
			displayNum_ = (displayNum_ % SceneManager::PLAYER_NUM) + 1;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			addIntervalTime_ += delta;

			//インターバル1秒ごとに数を１ずつ増やしていく
			if (addIntervalTime_ > INTERVAL_TIME)
			{
				addIntervalTime_ = 0.0f;
				displayNum_ = (displayNum_ % SceneManager::PLAYER_NUM) + 1;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		addIntervalTime_ = INTERVAL_TIME;
		isPress_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!isPress_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			isPress_ = true;

			//人数を１削除(中身は1～4に収める)
			displayNum_ = (displayNum_ + 3) % SceneManager::PLAYER_NUM;
			if (displayNum_ == 0)displayNum_ = SceneManager::PLAYER_NUM;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			addIntervalTime_ += delta;

			//インターバル1秒ごとに数を１ずつ減らしていく
			if (addIntervalTime_ > INTERVAL_TIME)
			{
				addIntervalTime_ = 0.0f;

				displayNum_ = (displayNum_ + 3) % SceneManager::PLAYER_NUM;
				if (displayNum_ == 0)displayNum_ = SceneManager::PLAYER_NUM;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;

			}
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		addIntervalTime_ = INTERVAL_TIME;
		isPress_ = false;
	}

	//スペースキー押下で決定&入力デバイス選択へ
	if (selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		snd.Play(SoundManager::SOUND::DECIDE_SELECT);

		//プレイヤー人数の設定
		data.Input(SceneManager::PLAY_MODE::USER, displayNum_);

		//ディスプレイの設定
		data.Input(DataBank::INFO::DHISPLAY_NUM, displayNum_);

		selectScene_.ChangeSelect(SelectScene::SELECT::NUMBER);
		ChangeSelect(SelectScene::SELECT::NUMBER);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV座標（テクスチャ座標）
	mesh_.vertex_[0].u = ((float)(displayNum_)-1.0f) / 4.0f;	mesh_.vertex_[0].v = 1.0f;	// 左下
	mesh_.vertex_[1].u = (float)(displayNum_) / 4.0f;			mesh_.vertex_[1].v = 1.0f;	// 右下
	mesh_.vertex_[2].u = ((float)(displayNum_)-1.0f) / 4.0f;	mesh_.vertex_[2].v = 0.0f;	// 左上
	mesh_.vertex_[3].u = (float)(displayNum_) / 4.0f;			mesh_.vertex_[3].v = 0.0f;	// 右上

	pointL_.mesh_.vertex_[0].u = 0.0f;	pointL_.mesh_.vertex_[0].v = 1.0f;	// 左下
	pointL_.mesh_.vertex_[1].u = 1.0f;	pointL_.mesh_.vertex_[1].v = 1.0f;	// 右下
	pointL_.mesh_.vertex_[2].u = 0.0f;	pointL_.mesh_.vertex_[2].v = 0.0f;	// 左上
	pointL_.mesh_.vertex_[3].u = 1.0f;	pointL_.mesh_.vertex_[3].v = 0.0f;	// 右上
}

void SelectImage::NumberUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();
	auto& snd = SoundManager::GetInstance();
	bool sound = false;

	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!isPress_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);

			isPress_ = true;

			//人数を１追加(中身は1～4に収める)
			playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			addIntervalTime_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			if (addIntervalTime_ > INTERVAL_TIME)
			{
				addIntervalTime_ = 0.0f;
				playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1 ;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		addIntervalTime_ = INTERVAL_TIME;
		isPress_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!isPress_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			isPress_ = true;

			//人数を１削除(中身は1～4に収める)
			playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
			if (playerNum_ == 0)playerNum_ = SceneManager::PLAYER_NUM;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			addIntervalTime_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			if (addIntervalTime_ > INTERVAL_TIME)
			{
				addIntervalTime_ = 0.0f;
				playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
				if (playerNum_ == 0)playerNum_ = SceneManager::PLAYER_NUM;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		addIntervalTime_ = INTERVAL_TIME;
		isPress_ = false;
	}

	//スペースキー押下で決定&入力デバイス選択へ
	if (selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		snd.Play(SoundManager::SOUND::DECIDE_SELECT);

		//プレイヤー人数の設定
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
		
		for (int i = 1; i <= playerNum_; i++)
		{
			//ディスプレイの設定
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

		if (playerNum_ > 1)
		{
			//プレイヤー2以上の場合、2P以上のコントローラーをPAD操作に設定
			for (int num = 2; num <= playerNum_; num++)
			{
				data.Input(SceneManager::CNTL::PAD, num);
			}
		}

		selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
		ChangeSelect(SelectScene::SELECT::OPERATION);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::LEFT)
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
	auto& snd = SoundManager::GetInstance();
	bool sound = false;

	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!isPress_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			isPress_ = true;

			//キーを押した際に選択しているものを反転(2種類しかないので)
			isPad_ = !isPad_;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			addIntervalTime_ += delta;

			//インターバル1秒ごとに数を１ずつ増やしていく
			if (addIntervalTime_ > INTERVAL_TIME)
			{
				addIntervalTime_ = 0.0f;
				isPad_ = !isPad_;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		addIntervalTime_ = INTERVAL_TIME;
		isPress_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!isPress_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			isPress_ = true;

			//キーを押した際に選択しているものを反転(2種類しかないので)
			isPad_ = !isPad_;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			addIntervalTime_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			if (addIntervalTime_ > INTERVAL_TIME)
			{
				addIntervalTime_ = 0.0f;
				isPad_ = !isPad_;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		addIntervalTime_ = INTERVAL_TIME;
		isPress_ = false;
	}

	//スペースキー押下で決定&役職選択へ
	if (selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		snd.Play(SoundManager::SOUND::DECIDE_SELECT);

		//1Pの操作の設定
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		(isPad_) ? selectScene_.Change1PDevice(SceneManager::CNTL::PAD) : selectScene_.Change1PDevice(SceneManager::CNTL::KEYBOARD);

		selectScene_.ChangeSelect(SelectScene::SELECT::ROLE);
		ChangeSelect(SelectScene::SELECT::ROLE);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.Get1PConfig() == SelectScene::KEY_CONFIG::LEFT)
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

void SelectImage::DisplayDraw(void)
{
	mesh_.DrawTwoMesh(*imgDisplayNum_);

	PointsDraw();
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

	//仮表記
	if (GetRole() == static_cast<int>(SceneManager::ROLE::MAGE))
	{
		
		readyMesh_.DrawTwoMesh(*imgComingSoon_);
	}
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
	auto& snd = SoundManager::GetInstance();
	bool sound = false;

	if (isReady_ == true)
	{
		if (input.config_ == SelectScene::KEY_CONFIG::CANCEL)
		{
			//キャンセル音
 			snd.Play(SoundManager::SOUND::CANCEL_SELECT);

			isReady_ = false;
		}
		//準備完了の時にキャラ選択を操作できないようにする
		return;
	}

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!isPress_)
		{
			//選択変更音
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);

			isPress_ = true;

			//役職を選択
			role_ = (role_ % SceneManager::PLAYER_NUM) + 1;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			addIntervalTime_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			if (addIntervalTime_ > INTERVAL_TIME)
			{
				addIntervalTime_ = 0.0f;
				role_ = (role_ % SceneManager::PLAYER_NUM) + 1;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound: sound;
			} 
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		addIntervalTime_ = INTERVAL_TIME;
		isPress_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!isPress_)
		{
			//選択変更音
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);

			isPress_ = true;

			//役職を選択する際選択数がじょうげんにいきそうになったらループするように
			role_ = (role_ + 3) % SceneManager::PLAYER_NUM;
			if (role_ == 0)role_ = SceneManager::PLAYER_NUM;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			addIntervalTime_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			if (addIntervalTime_ > INTERVAL_TIME)
			{
				addIntervalTime_ = 0.0f;
				role_ = (role_ + 3) % SceneManager::PLAYER_NUM;
				if (role_ == 0)role_ = SceneManager::PLAYER_NUM;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		addIntervalTime_ = INTERVAL_TIME;
		isPress_ = false;
	}

	//スペースキー押下で役職決定
	if (role_ != 3	&&
		input.config_ == SelectScene::KEY_CONFIG::DECIDE)
	{
		//決定音
		snd.Play(SoundManager::SOUND::SCENE_CHANGE_1);

		//役職の設定
 		data.Input(static_cast<SceneManager::ROLE>(role_ ), i + 1);

		isReady_ = true;
	}

	//選択する矢印を変更する
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
	mesh_.vertex_[0].u = 0.0f;	mesh_.vertex_[0].v = ((float)(role_)) / 4.0f;	// 左下
	mesh_.vertex_[1].u = 1.0f;	mesh_.vertex_[1].v = ((float)(role_)) / 4.0f;	// 右下
	mesh_.vertex_[2].u = 0.0f;	mesh_.vertex_[2].v = ((float)(role_)-1.0f) / 4.0f;	// 左上
	mesh_.vertex_[3].u = 1.0f;	mesh_.vertex_[3].v = ((float)(role_)-1.0f) / 4.0f;	// 右上
}

void SelectImage::ChangeSelect(const SelectScene::SELECT _state)
{
	//状態遷移
	state_ = _state;

	// 各状態遷移の初期処理
	stateChanges_[state_]();
}

void SelectImage::LoadImages(void)
{
	//画像の読み込み

	//人数選択用
	imgPlayerNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_NUM).handleIds_;
	if (imgPlayerNum_ == nullptr)return;	//中身がなかったら中止
	
	//ディスプレイ数選択用
	imgDisplayNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DISPLAY_NUM).handleIds_;
	if (imgDisplayNum_ == nullptr)return;
	
	//役職選択用
	imgRoleNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARA_PARAMS).handleIds_;
	if (imgRoleNum_ == nullptr)return;
	
	//使用デバイス選択用
	imgDeviceNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DEVICE).handleIds_;
	if (imgDeviceNum_ == nullptr)return;
	
	//矢印
	imgRightPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RIGHT_POINT).handleIds_;
	imgLeftPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEFT_POINT).handleIds_;
	if (imgRightPoint_ == nullptr)return;
	if (imgLeftPoint_ == nullptr)return;
	
	//準備完了用
	imgReady_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::READY).handleIds_;
	if (imgReady_ == nullptr)return;
	//魔法使い用のComingSoon画像
	imgComingSoon_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMING_SOON).handleIds_;
	if (imgComingSoon_ == nullptr)return;
}

void SelectImage::LoadSounds(void)
{
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//SE読み込み

	//準備完了SE
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::SCENE_CHANGE_1,
		res.Load(ResourceManager::SRC::SCENE_CHANGE_SE1).handleId_);
	int ret = res.Load(ResourceManager::SRC::SCENE_CHANGE_SE1).handleId_;
	if (ret == -1)return;
	
	//音量調節
	snd.AdjustVolume(SoundManager::SOUND::SCENE_CHANGE_1, 256);

	//キャンセルSE
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::CANCEL_SELECT,
		res.Load(ResourceManager::SRC::CANCEL_SELECT).handleId_);
	ret = res.Load(ResourceManager::SRC::CANCEL_SELECT).handleId_;
	if (ret == -1)return;
	
	//音量調節
	snd.AdjustVolume(SoundManager::SOUND::CANCEL_SELECT, 256);

	//選択変更SE
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::CHANGE_SELECT,
		res.Load(ResourceManager::SRC::CHANGE_SELECT).handleId_);
	ret = res.Load(ResourceManager::SRC::CHANGE_SELECT).handleId_;
	if (ret == -1)return;

	//音量調節
	snd.AdjustVolume(SoundManager::SOUND::CHANGE_SELECT, 70);

	//決定SE
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DECIDE_SELECT,
		res.Load(ResourceManager::SRC::DECIDE_SELECT).handleId_);
	ret = res.Load(ResourceManager::SRC::DECIDE_SELECT).handleId_;
	if (ret == -1)return;
	
	//音量調節
	snd.AdjustVolume(SoundManager::SOUND::DECIDE_SELECT, 80);
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

	readyMesh_.vertex_[0].pos = READYMESH_LEFT_BOT;	// 左下
	readyMesh_.vertex_[1].pos = READYMESH_RIGHT_BOT;	// 右下
	readyMesh_.vertex_[2].pos = READYMESH_LEFT_TOP;// 左上
	readyMesh_.vertex_[3].pos = READYMESH_RIGHT_TOP;	// 右上

	pointL_.mesh_.vertex_[0].pos = { POINT_LEFT_X, POINT_UNDER_Y, POINT_TOP_Z };	// 左下
	pointL_.mesh_.vertex_[1].pos = { POINT_RIGHT_X, POINT_UNDER_Y, POINT_TOP_Z };	// 右下
	pointL_.mesh_.vertex_[2].pos = { POINT_LEFT_X, POINT_TOP_Y, POINT_UNDER_Z };	// 左上
	pointL_.mesh_.vertex_[3].pos = { POINT_RIGHT_X, POINT_TOP_Y, POINT_UNDER_Z };	// 右上

	pointR_.mesh_.vertex_[0].pos = { -POINT_RIGHT_X, POINT_UNDER_Y, POINT_TOP_Z };	// 左下
	pointR_.mesh_.vertex_[1].pos = { -POINT_LEFT_X, POINT_UNDER_Y, POINT_TOP_Z };	// 右下
	pointR_.mesh_.vertex_[2].pos = { -POINT_RIGHT_X, POINT_TOP_Y, POINT_UNDER_Z };	// 左上
	pointR_.mesh_.vertex_[3].pos = { -POINT_LEFT_X, POINT_TOP_Y, POINT_UNDER_Z };	// 右上
}

void SelectImage::ChangeStateDisplay(void)
{
	stateUpdate_ = std::bind(&SelectImage::DisplayUpdate, this);
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
	ReductionVertexPos();
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
