#include<EffekseerForDXLib.h>
#include<cassert>
#include "EffectManager.h"

/// <summary>
/// エフェクトの追加
/// </summary>
/// <param name="_efc">エフェクト種類名</param>
/// <param name="_data">エフェクトのデータ</param>
void EffectManager::Add(const EFFECT& _efc, int _data)
{
	//連想配列内にすでに要素が入っているかを検索
	//入っていたら処理終了
	if (effectRes_.find(_efc) != effectRes_.end())return;

	//新しく要素を追加する
	effectRes_.emplace(_efc, std::move(_data));
}


/// <summary>
/// エフェクトの再生
/// </summary>
/// <param name="_efc">エフェクト種類名</param>
/// <param name="_pos">再生位置</param>
/// <param name="_qua">角度</param>
/// <param name="_size">大きさ</param>
void EffectManager::Play(const EFFECT& _efc, const VECTOR& _pos, const Quaternion& _qua, const float& _size)
{
	//要質問
	//(シングルトン化されているエフェクトマネージャにおいてPlayIdは臨時変数の一つだけでいいのか？)
	//(PlayIdも連想配列にすべきなのか？そもそもPlayIdが必要なのか？)
	
	//配列内に要素が入っていないかを検索
	if (effectPlay_.find(_efc) == effectPlay_.end()) {
		//入っていないとき要素を追加する
		effectPlay_.emplace(_efc, PlayEffekseer3DEffect(effectRes_[_efc]));
	}else {
		//入っていたら元あるやつに上書きする
		effectPlay_[_efc] = PlayEffekseer3DEffect(effectRes_[_efc]);
	}

	//その他各種設定
	//大きさ
		SetScalePlayingEffekseer3DEffect(effectPlay_[_efc], _size, _size, _size);
	//角度
		SetRotationPlayingEffekseer3DEffect(effectPlay_[_efc], _qua.ToEuler().x, _qua.ToEuler().y, _qua.ToEuler().z);
	//位置
		SetPosPlayingEffekseer3DEffect(effectPlay_[_efc], _pos.x, _pos.y, _pos.z);

}


/// <summary>
/// エフェクトの再生停止
/// </summary>
/// <param name="_efc">エフェクト種類名</param>
void EffectManager::Stop(const EFFECT& _efc)
{
	//配列内に入っていないものを停止しようとしたら警告
	if (effectPlay_.find(_efc) == effectPlay_.end())assert("設定していないエフェクトを停止しようとしています。");
	//再生停止
	StopEffekseer3DEffect(effectPlay_[_efc]);
}

//解放処理
void EffectManager::Release(void)
{
	//配列内の要素を全て消去
	//元々のデータはリソースマネージャが持っているのでおｋ。
	//念のためあとでこれについても聞く(_dataのstd::moveについて)
	effectRes_.clear();
}
