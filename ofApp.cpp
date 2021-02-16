#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 100, true, true, true);

	this->cap.open("D:\\MP4\\Pexels Videos 2880.mp4");
	this->cap_size = cv::Size(1280, 720);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(cv::Size(this->image.getWidth(), this->image.getHeight()), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->gray_image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_GRAYSCALE);
	this->gray = cv::Mat(this->gray_image.getHeight(), this->gray_image.getWidth(), CV_MAKETYPE(CV_8U, this->gray_image.getPixels().getNumChannels()), this->gray_image.getPixels().getData(), 0);

	this->edge_image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_GRAYSCALE);
	this->edge = cv::Mat(this->edge_image.getHeight(), this->edge_image.getWidth(), CV_MAKETYPE(CV_8U, this->edge_image.getPixels().getNumChannels()), this->edge_image.getPixels().getData(), 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat cap_frame;
	this->cap >> cap_frame;
	if (cap_frame.empty()) {

		cap.set(cv::CAP_PROP_POS_FRAMES, 1);
		return;
	}
	cv::resize(cap_frame, this->frame, this->cap_size);
	cv::cvtColor(this->frame, this->frame, cv::COLOR_BGR2RGB);
	cv::cvtColor(this->frame, this->gray, cv::COLOR_RGB2GRAY);
	
	this->frame.forEach<cv::Vec3b>([](cv::Vec3b& p, const int* position) -> void {
		
		p[0] = floor(p[0] / 30) * 30;
		p[1] = floor(p[1] / 30) * 330;
		p[2] = floor(p[2] / 30) * 30;
	});

	cv::Canny(this->gray, this->edge, 10, 200);

	cv::Mat color_edge;
	cv::cvtColor(this->edge, color_edge, cv::COLOR_GRAY2RGB);

	cv::subtract(this->frame, color_edge, this->frame);

	this->image.update();
	this->gray_image.update();
	this->edge_image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	ofSetColor(255);
	this->image.draw(0, 0);

	ofSetColor(239);
	string word = "PCD 2021";
	this->font.drawStringAsShapes(word, this->font.stringWidth(word) * -0.5, this->font.stringHeight(word) * 0.5);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}