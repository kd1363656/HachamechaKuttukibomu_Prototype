#pragma once

template <class OwnerType>
class StateBase;

// 凡庸的なステートパターンを管理するクラス
template <class OwnerType>
class StateMachine
{
public:

	StateMachine () : 
		m_owner   (nullptr),
	    m_nowState(nullptr),
		m_changeStateFunction([](){})
	{}
	~StateMachine() = default;

	void Start(OwnerType* Owner)
	{
		m_owner               = Owner;
		// こう記述すると何の処理もしない関数にすることができる
		m_changeStateFunction = [](){};
	}

	// 切り替えるステートとコンストラクタに渡す値を引数とする
	template<class StateType>
	void ChangeState()
	{
		// "ChangeState"関数を呼び出すことでステートの変更命令を格納
		m_changeStateFunction = [&]()
		{
			if (!m_owner)return;

			if(m_nowState)
			{
				m_nowState->Exit(m_owner);
				m_nowState = nullptr;
			}

			m_nowState = std::make_shared<StateType>();

			if (!m_nowState)
			{
				assert(false && "ステートが生成されませんでした");
			}

			m_nowState->SetMachine(this);
			m_nowState->Enter(m_owner);
		};
	}

	void Update()
	{
		// ステートの切り替えが行われていたらステートを切り替える
		// ステート生成の遅延処理
		// 遅延初期化させることで移動がステートの切り替えが滑らかなように見える
		m_changeStateFunction();
		// 生成するステートをクリア
		m_changeStateFunction = [](){};

		if(m_nowState)
		{
			m_nowState->Update(m_owner);
		}
	}

private:

	OwnerType* m_owner;

	std::shared_ptr<StateBase<OwnerType>> m_nowState;

	std::function<void()> m_changeStateFunction;
};