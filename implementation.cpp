//#include "NURBSParaboloid.h"
//#include "NURBSsurface.h"
//#include "Paraboloid.h"
#include <OpenSim/OpenSim.h>
using namespace SimTK;
using namespace OpenSim;

int main() {
	Model model;
	model.setName("bicep_curl");
	model.setUseVisualizer(true);

	OpenSim::SpatialTransform  SpTrans;// = new OpenSim::SpatialTransform();
	SpTrans.getFunctions();

	// Create two links, each with a mass of 1 kg, center of mass at the body's
	// origin, and moments and products of inertia of zero.
	OpenSim::Body* patella = new OpenSim::Body("patella", 1, Vec3(0), Inertia(0));
	OpenSim::Body* radius = new OpenSim::Body("radius", 1, Vec3(0), Inertia(0));

	// Connect the bodies with pin joints. Assume each body is 1 m long.
	CustomJoint* Patella = new CustomJoint("patella",
		// Parent body, location in parent, orientation in parent.
		model.getGround(), Vec3(0), Vec3(0),
		// Child body, location in child, orientation in child.
		*patella, Vec3(0, 1, 0), Vec3(0),
		// Spatial Transform
		SpTrans);






	// Add components to the model.
	model.addBody(patella);   
	model.addJoint(Patella);  

	// Add a console reporter to print the muscle fiber force and elbow angle.
	ConsoleReporter* reporter = new ConsoleReporter();
	reporter->set_report_time_interval(1.0);
	model.addComponent(reporter);

	// Add display geometry.
	Sphere bodyGeometry(0.1);
	bodyGeometry.setColor(Gray);
	// Attach an ellipsoid to a frame located at the center of each body.
	PhysicalOffsetFrame* patellaCenter = new PhysicalOffsetFrame(
		"patellaCenter", *patella, Transform(Vec3(0, 0.5, 0)));
	patella->addComponent(patellaCenter);


	// Configure the model.
	State& state = model.initSystem();
	// Fix the shoulder at its default angle and begin with the elbow flexed.
	Patella->getCoordinate().setValue(state, 0.5 * Pi);      //.setLocked(state, true);
	//elbow->getCoordinate().setValue(state, 0.5 * Pi);
	//model.equilibrateMuscles(state);

	// Configure the visualizer.
	model.updMatterSubsystem().setShowDefaultGeometry(true);
	Visualizer& viz = model.updVisualizer().updSimbodyVisualizer();
	viz.setBackgroundType(viz.SolidColor);
	viz.setBackgroundColor(White);

	// Simulate.
	simulate(model, state, 10.0);

	return 0;
};