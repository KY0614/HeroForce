#include "EffectManager.h"

/// <summary>
/// エフェクトの追加
/// </summary>
/// <param name="_efc">エフェクト種類名</param>
/// <param name="_data">エフェクトのデータ</param>
void EffectManager::Add(const EFFECT& _efc, int _data)
{
	//連想配列内にすでに要素が入っていないかを検索
	//入っていたら処理終了
	if (effectRes_.find(_efc) != effectRes_.end())return;

	//新しく要素を追加する
	effectRes_.emplace(_efc, std::move(_data));
}
