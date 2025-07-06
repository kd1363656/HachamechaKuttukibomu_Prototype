#pragma once

// 前方宣言にも"template"を持たせることができる
template <class OwnerType>
class StateMachine;

// =============================================
// 凡庸的なステートパターン
// 中身はステートの切り替え条件と処理のみを書く
// 変数の保存はステートを使っているクラスで行う
// =============================================
template <class OwnerType>
class StateBase
{
public:

	StateBase() : 
		m_owner       (nullptr) , 
	    m_stateMachine(nullptr) 
	{}
	~StateBase() = default;

// "protected"
protected:

	// "StateMachine"クラスから"StateBase"クラスのプライベートメンバにアクセス可能
	// ステートマシンクラスのみ"private"メンバにアクセス出来て便利
	friend class StateMachine<OwnerType>;
	
	virtual void Enter (OwnerType* Owner){Owner;}
	virtual void Update(OwnerType* Owner){Owner;}
	virtual void Exit  (OwnerType* Owner){Owner;}

private:

	void SetMachine(StateMachine<OwnerType>* Machine)
	{	
		// ステートが切り替わればコンストラクタが実行され"nullptr"になるが
		// 安全のために"nullptr"チェックを入れる
		if (!m_stateMachine)
		{
			m_stateMachine = Machine;
		}
	}

	bool CanProgress()
	{
		return m_owner && m_stateMachine;
	}

protected:

	StateMachine<OwnerType>* m_stateMachine;

	OwnerType* m_owner;
};