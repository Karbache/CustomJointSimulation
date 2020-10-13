//#include "NURBSParaboloid.h"
//#include "NURBSsurface.h"
#include "Paraboloid.h"
#include <OpenSim/OpenSim.h>
using namespace SimTK;
using namespace OpenSim;

int main() {
	Model model;
	model.setName("patella_movement");
	model.setUseVisualizer(true);

	// Define patella coordinates and axes for custom joint
	SpatialTransform spTrans;
	OpenSim::Array<std::string> coordNames;
	coordNames.append("patella_qx");    coordNames.append("patella_qy"); coordNames.append("patella_qz");
	spTrans[3].setCoordinateNames(OpenSim::Array<std::string>(coordNames[0], 1, 1));
	spTrans[3].setFunction(new LinearFunction(5,0));
	spTrans[4].setCoordinateNames(OpenSim::Array<std::string>(coordNames[1], 1, 1));
	spTrans[4].setFunction(new LinearFunction());
	spTrans[5].setCoordinateNames(OpenSim::Array<std::string>(coordNames[2], 1, 1));
	spTrans[5].setFunction(new LinearFunction());

	// Create two links, each with a mass of 1 kg, center of mass at the body's
	// origin, and moments and products of inertia of zero.
	OpenSim::Body* patella = new OpenSim::Body("patella", 0.01, Vec3(0), Inertia(0));

	// Connect the bodies with pin joints. Assume each body is 1 m long.
	CustomJoint* Patella = new CustomJoint("patella",
		// Parent body, location in parent, orientation in parent.
		model.getGround(), Vec3(0), Vec3(0),
		// Child body, location in child, orientation in child.
		*patella, Vec3(0, 0, 0), Vec3(0),
		// Spatial Transform
		spTrans);






	// Add components to the model.
	model.addBody(patella);   
	model.addJoint(Patella);  

	// Add a console reporter to print the muscle fiber force and elbow angle.
	ConsoleReporter* reporter = new ConsoleReporter();
	reporter->set_report_time_interval(1.0);
	model.addComponent(reporter);

	// Add display geometry.
	OpenSim::Geometry* bodyGeometry =  new Sphere(0.5);
	//bodyGeometry.setColor(Black);
	// Attach an sphere to a frame located at the center of each body.
	PhysicalOffsetFrame* patellaCenter = new PhysicalOffsetFrame(
		"patellaCenter", *patella, Transform(Vec3(0, 0, 0)));
	patella->addComponent(patellaCenter);
	patella->attachGeometry(bodyGeometry);


	// Configure the model.
	State& state = model.initSystem();
	// Fix the shoulder at its default angle and begin with the elbow flexed.
	Patella->getCoordinate(0).setSpeedValue(state, -0.5);
	Patella->getCoordinate(1).setSpeedValue(state, 0.5);
	Patella->getCoordinate(2).setSpeedValue(state, 0.5);
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