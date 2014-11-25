#include "ShaderProgramText.h"
#include "Logger.hpp"
#include "GLHelpers.h"
#include "platform_asset.h"
#include <string>

ShaderProgramText::ShaderProgramText(glm::mat4 *viewMatrix, glm::mat4 *projectionMatrix)
	: ShaderProgram(viewMatrix, projectionMatrix)
{
	mProgramHandle = GLHelpers::build_program_from_assets("Text.vsh", "Text.fsh");

	addAttribute("a_texcoord");
	addUniform("u_textureUnit");
	addUniform("u_color");
    addUniform("u_MVPMatrix");

    glGenBuffers(1, &mVbo);
}

ShaderProgramText::~ShaderProgramText() {
    // TODO Auto-generated destructor stub
}

struct texVertice2d
{
		GLfloat x;
		GLfloat y;
		GLfloat s;
		GLfloat t;
};

void ShaderProgramText::draw(const std::string& text, glm::mat4* modelMatrix, const FontAtlas& mFontAtlas)
{
    glUseProgram(mProgramHandle);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mFontAtlas.handle());
    glUniform1i(mUniforms["u_textureUnit"], 0);

    glm::mat4 MVPMatrix = (*mProjectionMatrix) * (*mViewMatrix) * (*modelMatrix);
    glUniformMatrix4fv(mUniforms["u_MVPMatrix"], 1, GL_FALSE, glm::value_ptr(MVPMatrix));

    GLfloat red[4] = { 1, 0, 0, 1 };
    glUniform4fv(mUniforms["u_color"], 1, red);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, mFontAtlas.handle());
    glUniform1i(mUniforms["u_textureUnit"], 0);

    /* Set up the VBO for our vertex data */

	texVertice2d coords[6 * text.length()];

	int n = 0;

	float x = 0;
	float y = 0;

	for (const char& c : text)
	{
		const FontAtlas::CharacterInfo& info = mFontAtlas.charInfo(c);
		float x2 =  x + info.bmpLeft;
		float y2 = -y - info.bmptop;
		float w = info.bmpWidth;
		float h = info.bmpHeight;

		/* Advance the cursor to the start of the next character */
		x += info.advanceX;
		y += info.advanceY;

		/* Skip glyphs that have no pixels */
		if(!w || !h)
			continue;

		coords[n++] = (texVertice2d){x2, -y2,
							 info.txOffset, 0};
		coords[n++] = (texVertice2d){x2 + w, -y2,
							 info.txOffset + info.bmpWidth / mFontAtlas.width(), 0};
		coords[n++] = (texVertice2d){x2, -y2 - h,
							 info.txOffset, info.bmpHeight / mFontAtlas.height() }; //remember: each glyph occupies a different amount of vertical space
		coords[n++] = (texVertice2d){x2 + w, -y2,
							 info.txOffset + info.bmpWidth / mFontAtlas.width(), 0};
		coords[n++] = (texVertice2d){x2,     -y2 - h,
							 info.txOffset, info.bmpHeight / mFontAtlas.height()};
		coords[n++] = (texVertice2d){x2 + w, -y2 - h,
							 info.txOffset + info.bmpWidth / mFontAtlas.width(), info.bmpHeight / mFontAtlas.height()};
	}

    glEnableVertexAttribArray(mAttributes["a_texcoord"]);
    glBindBuffer(GL_ARRAY_BUFFER, mVbo);
    glVertexAttribPointer(mAttributes["a_texcoord"], 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW);

	glDrawArrays(GL_TRIANGLES, 0, n);
	glDisableVertexAttribArray(mAttributes["a_texcoord"]);
}
