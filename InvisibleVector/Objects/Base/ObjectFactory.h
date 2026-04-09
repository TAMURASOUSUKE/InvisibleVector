#pragma once
#include "Vector_Dxlib.h"
#include "ObjectManager.h"

/*
	使い方の説明 : このファクトリーの責任はオブジェクトの生成です。
	そのためPlayerなどのファイルをインクルードする必要があります。
	上記の「具体をインクルード」の下あたりに自分が作ったオブジェクトファイルをインクルードしてください。

	次に下の関数例に倣って「Create自分が作ったオブジェクトの名前」で関数を作りましょう
	引数は基本的には自分が作ったオブジェクトが生成されるときに必要なもの(コンストラクタと同じでいい)を設定してください。

	これができれば.hでの関数宣言は完了です。次にObjectFactory.cppを見てください。
*/

class Player;	// Player側でFactoryのincludeを行っているため、こちらではPlayerの前方宣言


// オブジェクトの生成を行う
class ObjectFactory
{
public:
	static void BindManager(ObjectManager& _objManager); // 各シーンが持つObjectManagerを受け取るため

	// テスト用作成関数
	static void CreateDebugObject(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive);
	static void CreateDebugGround(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive);
	static void CreateDebugGround(const Vector3& position, const Vector3& rotation, const Vector3& scale, const bool& isActive, const Vector3& boxSize);
private:
	static ObjectManager* targetManager; // どのシーンのオブジェクトマネージャーを使うかをバインドするための変数(参照に使うため生でよい)

};