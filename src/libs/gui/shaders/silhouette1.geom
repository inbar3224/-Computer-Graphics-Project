#version 330

layout (triangles_adjacency) in;
layout (line_strip, max_vertices = 6) out;

bool isFrontFacing(vec3 a, vec3 b, vec3 c) {	
	return ((a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y)) >= 0;
}

void EmitLine(int StartIndex, int EndIndex) {
    gl_Position = gl_in[StartIndex].gl_Position;
    EmitVertex();

    gl_Position = gl_in[EndIndex].gl_Position;
    EmitVertex();

    EndPrimitive();
}

void main() {
    vec3 a  = gl_in[0].gl_Position.xyz;
	vec3 ab = gl_in[1].gl_Position.xyz;
	vec3 b  = gl_in[2].gl_Position.xyz;
	vec3 bc = gl_in[3].gl_Position.xyz;
	vec3 c  = gl_in[4].gl_Position.xyz;
	vec3 ca = gl_in[5].gl_Position.xyz;

	bool primary = isFrontFacing(a, b, c);
	bool first = isFrontFacing(a, ab, b);
	bool second = isFrontFacing(b, bc, c);
	bool third = isFrontFacing(c, ca, a);

	if(primary == true) {
		// first
		if(first == true) {
			EmitLine(0, 1);
			EmitLine(1, 2);
		}
		else if(first == false) {
			EmitLine(0, 2);
		}

		// second
		if(second == true) {
			EmitLine(2, 3);
			EmitLine(3, 4);
		}
		else if(second == false) {
			EmitLine(2, 4);
		}
		
		// third
		if(third == true) {
			EmitLine(4, 5);
			EmitLine(5, 0);
		}
		else if(third == false) {
			EmitLine(4, 0);
		}
	}
}

// tryouts:
// ------------------------------------ 1 -----------------------------------
//if(first == false && second == false && third == false) {
	//EmitLine(0, 2);
	//EmitLine(2, 4);
	//EmitLine(4, 0);
//}
//if(first == true && second == false && third == false) {
	//EmitLine(0, 1);
	//EmitLine(1, 2);
	//EmitLine(2, 4);
	//EmitLine(4, 0);
//}
//if(first == false && second == true && third == false) {
	//EmitLine(0, 2);
	//EmitLine(2, 3);
	//EmitLine(3, 4);
	//EmitLine(4, 0);
//}
//if(first == false && second == false && third == true) {
	//EmitLine(0, 2);
	//EmitLine(2, 4);
	//EmitLine(4, 5);
	//EmitLine(5, 0);
//}

// ------------------------------------ 2 -----------------------------------
//if (isFrontFacing(a, b, c)) {		
	//if (!isFrontFacing(a, ab, b)) {
	//	EmitLine(0, 2);
	//}

	//if (!isFrontFacing(b, bc, c)) {
	//	EmitLine(2, 4);
	//}

	//if (!isFrontFacing(c, ca, a)) {
	//	EmitLine(4, 0);
	//}
//}

// ------------------------------------ 3 -----------------------------------		
//EmitLine(0, 1);
//EmitLine(0, 2);
//EmitLine(0, 4);
//EmitLine(0, 5);
	
//EmitLine(1, 2);

//EmitLine(2, 3);
//EmitLine(2, 4);

//EmitLine(3, 4);

//EmitLine(4, 5);