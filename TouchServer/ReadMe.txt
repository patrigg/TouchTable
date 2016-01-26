========================================================================
    CONSOLE APPLICATION : TouchServer Project Overview
========================================================================
This application implements touch tracking for the ASUS Xtion depth sensor.
It is based on subtracting a static background from depth images and segmenting
regions within a specific depth range. Touches are tracked by their center
points and events for touch, move and release are emitted.

Data can either be sent binary or in json format to a UDP socket or the 
standard output stream. The application may be reconfigured at runtime by
sending commands in JSON format.

///Command-line arguments://////////////////////////////////////////////////
The following command line arguments are supported. Default arguments are indicated
by (default) and default values with [param:value].
// Protocol:
	console (default)
		writes data to standard output stream and receives configuration
		commands from standard input.
	udp [host] [port]
		sends UDP data to a specific [host] at some [port].

// Data format:
	json (default)
		data is sent as json objects.
	binary
		data is sent as binary packets. Runtime reconfiguration is not fully
		functional since command acknowledgements are sent as json interleaved
		with the binary stream.

// Coordinate System:
	horizontal_flip
		touch point coordinates are flipped horizontally.
	vertical_flip
		touch point coordinates are flipped vertically.

// Tracking:
	min_blob_size [size:40]
		sets the minimum [size] in pixels for a single touch region to be detected.
		Setting this to larger values cancels out false positives due to noise in
		the depth images.
	depth_threshold [min:30] [max:80]
		sets the depth range in millimeters after subtracting the static background
		in which pixels are recognized as part of touch regions. Setting the minimum
		value too low increases false positives due to noise. Setting the maximum
		too high makes the arms also part of touch regions and thus causes touch
		center points not to be on the actual touch location.
	depth_threshold_min [min]
		same as depth_threshold but only sets the minimum value.
	depth_threshold_max [max]
		same as depth_threshold but only sets the maximum value.

// Stream:
	stopped
		data stream is not started after lauch and no packets are sent to the stream.
	delay [time:0]
		[time] in milliseconds to wait between two depth frames. Setting this value
		to zero causes tracking to be done as fast as possible at max. 60 fps.
		Used to reduce CPU load when fast response times are not needed.
	
///Runtime reconfiguration://////////////////////////////////////////////////
Reconfiguration at runtime is done by sending JSON objects to the server.
The whole JSON object has to be either in a single UDP packet or a single line
without any line breaks. All members in the JSON object are optional. Each
message is acknowledged by the server.

// General:
	"message_id": number
		is set, acknowledgement to this command will contain the same message_id

// Data Format:
	"data_format": "json" | "binary"
		see "Data Format" command-line argument.

// Coordinate System:
	"horizontal_flipped": boolean
	"vertical_flipped": boolean
		if true see "Coordinate System" command-line arguments.

// Tracking:
	"min_blob_size": number
	"depth_threshold_max": {"min": number, "max": number}
	"depth_threshold_min": number
	"depth_threshold_max": number
		see "Tracking" command-line arguments.

// Stream:
	"delay": number
		see "delay" command-line argument.
	"start": boolean
		if true starts tracking, else does nothing
	"stop": boolean
		if true immediately stops tracking, else does nothing
	"remove_background" : boolean
		if set to true replaces static background depth image with current image
		else does nothing

// Query:
	"get_configuration": boolean
		if true returns the current configuration instead of a simple reply

///JSON Packet format:////////////////////////////////////////////////////////////

// Simple Reply:
Is returned after runtime reconfiguration packet is received.

	"message_id": number
		"message_id" value from the runtime reconfiguration.
		Or "-1" if none was given.

	"status": "ok" | "failed"
		"ok" if JSON was successfully parsed. Unknown members are silently ignored.
		"failed" if there was a problem parsing the JSON object.

// Current Configuration Reply:
Contains all members of simple reply plus members listed below.

	"horizontal_flipped": boolean
	"vertical_flipped": boolean
	"min_blob_size": number
	"depth_threshold_max": {"min": number, "max": number}
	"depth_threshold_min": number
	"depth_threshold_max": number
	"delay": number
		see Runtime reconfiguration section.

	"running": boolean
		true if the server is streaming data

// Data Packet:
Each tracked point in json mode is sent via a separate JSON object with
the following members.

	"type": "Touch" | "Move" | "Release"
		"Touch" occurs when a touch point is first detected
		"Move" occurs continuously when a point is tracked over time
		"Release" occurs when tracking of the point is lost

	"frame_id": number
		Id of the frame in which the point was tracked. This number is 
		incremented with each frame from the depth camera.

	"id": number
		Unique number for the touch point which stays constant during tracking.

	"position": { "x": number, "y": number}
		Position of the touch point center in screen coordinates of the depth
		camera. This is influenced by the flip_* options.

///Binary Packet format://////////////////////////////////////////////////////////
Each tracked point in binary mode is sent via constant size packet with the
following format and fields. Fields have mostly the same semantics as in the json 
data packets described above. Variations are described below.

size in bytes: |1|____4_____|_4__|__1___|_____4______|_____4______|
field:         |b| frame_id | id | type | position.x | position.y |

	b:
		packet start and type indicator, is always "1"

	type:
		the type field is an enum with the following values:
		Touch:   1
		Move:    2
		Release: 3