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

	static void CreateFloatingBox(Vector2 _randomStartPos, Vector2 _randomEndPos, unsigned int _priority, int _handle);
	static void CreateChangeSceneLogo(Vector2 _pos, unsigned int _priority, int handle);

	static void CreateLogo(Vector2 _pos, unsigned int _priority, int _handle);
	static void CreateBackGround(Vector2 _pos, unsigned int _priority, int _handle);

	static Player* CreatePlayer(Vector3 _position, float _radius, int _handle);
	static void CreateCamera(Vector3 _position, Player* _player); // カメラの生成
	static void CreateStage();

	static void CreateReticle(unsigned int _priority, int _handle);

	static void CreateBullet(Vector3 _pos, Vector3 _start_pos, Vector3 _direction_vec, float _radius);

	static void CreateScore(Vector2 _graphPos, Vector2 _fontPos, unsigned int _priority, int _rankAHandle, int _rankBHandle, int _rankCHandle, int _fontHandle);
private:
	static ObjectManager* targetManager; // どのシーンのオブジェクトマネージャーを使うかをバインドするための変数(参照に使うため生でよい)

};