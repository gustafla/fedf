#ifndef VEC_HPP
#define VEC_HPP

struct vec2 {
	vec2(float x, float y) {
		values[0]=x;
		values[1]=y;
	}
	
	float x() {
		return values[0];
	}
	
	float y() {
		return values[1];
	}
	
	vec2 xy() {
		return vec2(values[0], values[1]);
	}
	
	vec2 yx() {
		return vec2(values[1], values[0]);
	}
	
	float operator [] (unsigned int i) const {
		return values[i];
	}
	
	float& operator [] (unsigned int i) {
		return values[i];
	}
	
	operator float*() const {
		return values;
	}
	
	vec2 operator+(vec2 const& l, vec2 const& r) {
		return vec2(l.x() + r.x(), l.y() + r.y());
	}
	
	vec2 operator-(vec2 const& l, vec2 const& r) {
		return vec2(l.x() - r.x(), l.y() - r.y());
	}
	
	vec2 operator*(vec2 const& l, vec2 const& r) {
		return vec2(l.x() * r.x(), l.y() * r.y());
	}
	
	vec2 operator/(vec2 const& l, vec2 const& r) {
		return vec2(l.x() / r.x(), l.y() / r.y());
	}
	
	void operator+=(vec2 const v) {
		values[0]+=v.x();
		values[1]+=v.y();
	}
	
	void operator-=(vec2 const v) {
		values[0]-=v.x();
		values[1]-=v.y();
	}
	
	void operator*=(vec2 const v) {
		values[0]*=v.x();
		values[1]*=v.y();
	}
	
	void operator/=(vec2 const v) {
		values[0]/=v.x();
		values[1]/=v.y();
	}
	
	void operator=(vec2 v) {
		values[0]=v.x();
		values[1]=v.y();
	}
	
	private float values[2];
}

struct vec3 {
	float x;
	float y;
	float z;
}

struct vec4 {
	float x;
	float y;
	float z;
	float w;
}

#endif
