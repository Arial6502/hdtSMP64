#pragma once

#include "hdtAABB.h"
#include "hdtBulletHelper.h"
#include <memory>

namespace hdt
{
	class SkinnedMeshBody;
	_CRT_ALIGN(16)
	struct SkinnedMeshBone :
		public RE::BSIntrusiveRefCounted
	{
		BT_DECLARE_ALIGNED_ALLOCATOR();

		SkinnedMeshBone(const RE::BSFixedString& name, btRigidBody::btRigidBodyConstructionInfo& ci);
		virtual ~SkinnedMeshBone();

		RE::BSFixedString m_name;
		float m_marginMultipler;
		float m_boudingSphereMultipler = 1.0f;
		float m_gravityFactor = 1.0f;
		float m_windFactor = 1.0f;  // Mapped to <wind-factor> in the XML. Acts as a multiplier for the global wind force applied to this bone (0.0 = no wind, 2.0 = double wind)

		btRigidBody m_rig;
		btTransform m_localToRig;
		btTransform m_rigToLocal;
		btQsTransform m_currentTransform;

		std::vector<RE::BSFixedString> m_canCollideWithBone;
		std::vector<RE::BSFixedString> m_noCollideWithBone;

		virtual void readTransform(float timeStep) = 0;
		virtual void writeTransform() = 0;

		// Moves the rig to follow an animated world transform, rescaling mass, inertia and the rig offset when the
		// bone's world scale changed since the last read.
		void readAnimatedTransform(const btQsTransform& animated, float timeStep, bool reset);

		// m_rigToLocal, before readAnimatedTransform scaled its offset. Constraints start at scale 1 and
		// apply the bone scale themselves, so they must be built from this.
		btTransform rigToLocalAtUnitScale() const
		{
			auto t = m_rigToLocal;
			t.getOrigin() /= m_currentTransform.getScale();
			return t;
		}

		void internalUpdate();

		bool canCollideWith(SkinnedMeshBone* rhs);
	};
}
