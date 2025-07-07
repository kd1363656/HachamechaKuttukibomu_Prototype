#pragma once

class Player;

class CameraBase : public KdGameObject
{
public:
	CameraBase()						{}
	virtual ~CameraBase()	override	{}

	virtual uint32_t GetFinalBaseTypeID()const override { return GameObjectID::GetTypeID<CameraBase>(); }

	void Init()				override;
	void PostLoadInit()     override;

	void Update()			override;
	void PreDraw()			override;

	virtual void ImGuiTransformInspector()override;

	void           LoadJsonData(const nlohmann::json Json) override;
	nlohmann::json SaveJsonData()						   override;

	void SetTarget(const std::shared_ptr<KdGameObject>& target);

	// 「絶対変更しません！見るだけ！」な書き方
	const std::shared_ptr<KdCamera>& GetCamera() const
	{
		return m_spCamera;
	}

	// 「中身弄るかもね」な書き方
	std::shared_ptr<KdCamera> WorkCamera() const
	{
		return m_spCamera;
	}

	const Math::Matrix GetRotationMatrix()const
	{
		return Math::Matrix::CreateFromYawPitchRoll(
		       DirectX::XMConvertToRadians(m_degAng.y),
		       DirectX::XMConvertToRadians(m_degAng.x),
		       DirectX::XMConvertToRadians(m_degAng.z));
	}

	const Math::Matrix GetRotationYMatrix() const
	{
		return Math::Matrix::CreateRotationY(
			   DirectX::XMConvertToRadians(m_degAng.y));
	}

	void RegistHitObject(const std::shared_ptr<KdGameObject>& object)
	{
		m_wpHitObjectList.push_back(object);
	}

protected:
	
	void ToggleIsMouseFree  ();
	void UpdateRotateByMouse();

	std::shared_ptr<KdCamera>					m_spCamera		= nullptr;
	std::vector<std::weak_ptr<KdGameObject>>	m_wpHitObjectList{};

	Math::Matrix								m_mLocalPos		= Math::Matrix::Identity;
	Math::Matrix								m_mRotation		= Math::Matrix::Identity;

	std::weak_ptr<Player>						m_player;

	// カメラ回転用角度
	Math::Vector3								m_degAng   = Math::Vector3::Zero;
	Math::Vector3							    m_location = Math::Vector3::Zero;
	
	// カメラ回転用マウス座標の差分
	POINT										m_FixMousePos{};

	// TODO
	bool									    m_isDebugMouseFree      = false;
	bool									    m_isPressDebugMouseFree = false;
};