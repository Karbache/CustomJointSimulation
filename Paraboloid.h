#ifndef OPENSIM_PARABOLOID_H_
#define OPENSIM_PARABOLOID_H_
/* -------------------------------------------------------------------------- *
 *                              OpenSim:  Paraboloid.h                        *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2017 Stanford University and the Authors                *
 * Author(s): Abdelkader Karbache                                             *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */


 // INCLUDES
#include <string>
#include <OpenSim/Common/Function.h>
#include <OpenSim/Common/FunctionAdapter.h>
#include "osimPluginDLL.h"


using namespace OpenSim;
using namespace std;

namespace OpenSim {

	//=============================================================================
	//=============================================================================
	/**
	 * A class for representing a paraboloid function.
	 *
	 * This class inherits from Function and can be used as input to
	 * any Component requiring a Function as input. Implements:
	 *  f(x,y) = ax^2 + by^2;
	 *
	 * @author Abdelkader Karbache
	 * @version 1.0
	 */
	class OSIMPLUGIN_API Paraboloid : public Function {
		OpenSim_DECLARE_CONCRETE_OBJECT(Paraboloid, Function);
	protected:
		//==============================================================================
		// PROPERTIES
		//==============================================================================
		OpenSim_DECLARE_PROPERTY(x_coefficient, double,
			"The x coefficient of the function");

		OpenSim_DECLARE_PROPERTY(y_coefficient, double,
			"The y coefficient of the function");
		//=============================================================================
		// METHODS
		//=============================================================================
	public:
		//Default construct, copy and assignment
		Paraboloid() {
			constructProperties();
		}

		// Convenience Constructor
		Paraboloid(double x_coefficient, double y_coefficient) : Paraboloid()
		{
			set_x_coefficient(x_coefficient);
			set_y_coefficient(y_coefficient);

		}
		virtual ~Paraboloid() {};

		//--------------------------------------------------------------------------
		// EVALUATION
		//--------------------------------------------------------------------------
		double calcValue(const SimTK::Vector& x) const override {
			return ((get_x_coefficient())*x[0] * x[0] + (get_y_coefficient())*x[1] * x[1]);
		}

		double calcDerivative(const std::vector<int>& derivComponents, const SimTK::Vector& x) const override
		{
			int nd = (int)derivComponents.size();
			if (nd < 1)
				return SimTK::NaN;

			if (derivComponents[0] == 0) {
				if (nd == 1)
					return 2 * x[0] * (get_x_coefficient());
				else if (derivComponents[1] == 0)
					return 2 * (get_x_coefficient());
			}
			else if (derivComponents[0] == 1) {
				if (nd == 1)
					return 2 * x[1] * (get_y_coefficient());
				else if (derivComponents[1] == 1)
					return 2 * (get_y_coefficient());
			}
			return 0;
		}

		SimTK::Function* createSimTKFunction() const override {
			return new FunctionAdapter(*this);
		}

		int getArgumentSize() const override { return 2; }
		int getMaxDerivativeOrder() const override { return 2; }

	private:
		void constructProperties() {
			constructProperty_x_coefficient(1.0);
			constructProperty_y_coefficient(1.0);
		}
		//=============================================================================
	};  // END class Paraboloid
	//=============================================================================
	//=============================================================================

} // end of namespace OpenSim

#endif  // OPENSIM_PARABOLOID_H_

