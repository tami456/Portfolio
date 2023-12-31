#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"
class Transform;

class Camera
{

public:

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// カメラ位置と注視点との相対座標
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -100.0f, 500.0f };

	//シェイク時間
	static constexpr float TIME_SHAKE = 2.0f;

	//シェイク幅
	//static constexpr float WIDTH_SHAKE = 3.0f;
	static constexpr float WIDTH_SHAKE = 5.0f;
	
	//シェイクスピード
	//static constexpr float SPEED_SHAKE = 30.0f;
	static constexpr float SPEED_SHAKE = 15.0f;

	//カメラモード
	enum class MODE
	{
		NONE,
		FREE,			//フリー
		FIXED_POINT,	//定点カメラ
		FOLLOW,			//追従カメラ
		FOLLOW_SPRING,	//バネ付き追従カメラ
		SHAKE,			//カメラシェイク(カメラ揺らし)
		LOCK,
	};

	// コンストラクタ
	Camera(void);

	// デストラクタ
	~Camera(void);

	// 初期化
	void Init(void);

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 更新
	void Update(void);

	// 描画前のカメラ設定
	void SetBeforeDraw(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSpring(void);
	void SetBeforeDrawShake(void);
	void SetBeforeDrawLock(void);
	
	// 描画
	void Draw(void);
	void DrawUI(void);

	void Release(void);

	// カメラ座標
	VECTOR GetPos(void) const;

	// 注視点座標
	VECTOR GetTargetPos(void) const;

	// カメラ方向
	VECTOR GetDir(void) const;

	//カメラモードの変更
	void ChangeMode(MODE mode);

	//追従対象の設定
	void SetFollowTarget(const Transform* target);
private:

	//追従対象
	const Transform* mTarget;

	//カメラモード
	MODE mMode;

	// カメラの位置
	VECTOR mPos;

	// カメラ角度
	Quaternion mQuaRot;

	// 注視点
	VECTOR mTargetPos;

	// カメラの上方向
	VECTOR mCameraUp;

	//速度
	VECTOR mVelocity;

	//カメラシェイク用
	//時間計測用
	float mStepShake;
	//カメラシェイク前の初期位置
	VECTOR mDefaultPos;
	//カメラを揺らす方向
	VECTOR mShakeDir;
};

