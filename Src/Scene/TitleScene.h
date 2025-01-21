#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"

class SkyDome;
class StageManager;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	//スカイドーム
	std::unique_ptr<SkyDome> skyDome_;

	//ステージ
	std::unique_ptr<StageManager> stage_;

	// タイトルロゴ
	int imgTitleLogo_;

	//ウィンドウの数を指定
	//int windowNum_;

	// スカイドーム用のダミー
	Transform spaceDomeTran_;


	// ロゴ描画
	void DrawLogo(void);

};
