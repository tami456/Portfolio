#pragma once

class Fader
{

public:

	// �t�F�[�h���i�ޑ���
	static constexpr float SPEED_ALPHA = 5.0f;

	// ���
	enum class STATE
	{
		NONE, 
		FADE_OUT,	// ���X�ɈÓ]
		FADE_IN		// ���X�ɖ��]
	};

	// ��Ԃ̎擾
	STATE GetState(void) const;

	// �t�F�[�h�������I�����Ă��邩
	bool IsEnd(void) const;

	// �w��t�F�[�h���J�n����
	void SetFade(STATE state);

	void Init(void);
	void Update(void);
	void Draw(void);

private:

	// ���
	STATE mState;

	// �����x
	float mAlpha;

	// ���(STATE)��ۂ����܂܏I��������s�����߁A
	// Update->Draw->Update��1�t���[������p
	bool mIsPreEnd;

	// �t�F�[�h�����̏I������
	bool mIsEnd;

};
