/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2021, Google Inc.
 *
 * camera_sensor_helper.h - Helper class that performs sensor-specific parameter computations
 */
#ifndef __LIBCAMERA_IPA_LIBIPA_CAMERA_SENSOR_HELPER_H__
#define __LIBCAMERA_IPA_LIBIPA_CAMERA_SENSOR_HELPER_H__

#include <stdint.h>

#include <memory>
#include <string>
#include <vector>

#include <libcamera/base/class.h>

namespace libcamera {

namespace ipa {

class CameraSensorHelper
{
public:
	CameraSensorHelper() = default;
	virtual ~CameraSensorHelper() = default;

	virtual uint32_t gainCode(double gain) const;
	virtual double gain(uint32_t gainCode) const;

protected:
	enum AnalogueGainType {
		AnalogueGainLinear = 0,
		AnalogueGainExponential = 2,
	};

	struct AnalogueGainConstants {
		AnalogueGainType type;
		int16_t m0;
		int16_t c0;
		int16_t m1;
		int16_t c1;
	};

	AnalogueGainConstants analogueGainConstants_;

private:
	LIBCAMERA_DISABLE_COPY_AND_MOVE(CameraSensorHelper)
};

class CameraSensorHelperFactory
{
public:
	CameraSensorHelperFactory(const std::string name);
	virtual ~CameraSensorHelperFactory() = default;

	static std::unique_ptr<CameraSensorHelper> create(const std::string &name);

	static void registerType(CameraSensorHelperFactory *factory);
	static std::vector<CameraSensorHelperFactory *> &factories();

protected:
	LIBCAMERA_DISABLE_COPY_AND_MOVE(CameraSensorHelperFactory)
	virtual CameraSensorHelper *createInstance() = 0;

	std::string name_;
};

#define REGISTER_CAMERA_SENSOR_HELPER(name, helper)		  \
class helper##Factory final : public CameraSensorHelperFactory	  \
{                                                                 \
public:                                                           \
	helper##Factory() : CameraSensorHelperFactory(name) {}	  \
								  \
private:                                                          \
	CameraSensorHelper *createInstance()			  \
	{							  \
		return new helper();				  \
	}							  \
};								  \
static helper##Factory global_##helper##Factory;

} /* namespace ipa */

} /* namespace libcamera */

#endif /* __LIBCAMERA_IPA_LIBIPA_CAMERA_SENSOR_HELPER_H__ */
