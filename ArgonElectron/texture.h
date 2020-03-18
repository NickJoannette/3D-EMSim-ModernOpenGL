#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
class Texture
{
public:
	Texture(const std::string& fileName);

	void Bind(unsigned int unit);


	virtual ~Texture();

protected:
private:
	Texture(const Texture& other) {}
	void operator = (const Texture& other) {}


};

#endif 

