#ifndef INCLUDED_MATRIX34_H
#define INCLUDED_MATRIX34_H

class Vector3;

class Matrix34{
public:
	Matrix34(); //ftHgÅRXgNg
	//ÚŪsņķŽ
	void setTranslation( const Vector3& );
	//gåkŽsņķŽ
	void setScaling( const Vector3& );
	//ņ]sņķŽX
	void setRotationX( double radian );
	//ņ]sņķŽY
	void setRotationY( double radian );
	//ņ]sņķŽZ
	void setRotationZ( double radian );
	//r[sņķŽ
	void setViewTransform( const Vector3& position, const Vector3& target );
	//xN^Æ|ŊZ
	void multiply( Vector3* out, const Vector3& in ) const;
	//sņÆ|ŊZ
	void operator*=( const Matrix34& );
	//ÚŪsņÆæZ
	void translate( const Vector3& );
	//gåkŽsņÆæZ
	void scale( const Vector3& );
	//Xņ]sņÆæZ
	void rotateX( double radian );
	//Yņ]sņÆæZ
	void rotateY( double radian );
	//Zņ]sņÆæZ
	void rotateZ( double radian );
private:
	double m00, m01, m02, m03;
	double m10, m11, m12, m13;
	double m20, m21, m22, m23;
};

#endif
