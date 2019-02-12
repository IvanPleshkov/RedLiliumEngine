#pragma once

#include <Core/Common.h>
#include <Render/RenderCommon.h>
#include <Render/Material.h>

namespace RED_LILIUM_NAMESPACE
{

class AssimpMaterial : public Material
{
public:
	~AssimpMaterial() override = default;

	 /** The texture is combined with the result of the diffuse
     *  lighting equation.
     */
	virtual u8 GetRequiredDiffuseTexture() { return 0; }
	virtual void SetDiffuseTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** The texture is combined with the result of the specular
     *  lighting equation.
     */
	virtual u8 GetRequiredSpecularTexture() { return 0; }
	virtual void SetSpecularTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** The texture is combined with the result of the ambient
     *  lighting equation.
     */
	virtual u8 GetRequiredAmbientTexture() { return 0; }
	virtual void SetAmbientTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** The texture is added to the result of the lighting
     *  calculation. It isn't influenced by incoming light.
     */
	virtual u8 GetRequiredEmissiveTexture() { return 0; }
	virtual void SetEmissiveTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** The texture is a height map.
     *
     *  By convention, higher gray-scale values stand for
     *  higher elevations from the base height.
     */
	virtual u8 GetRequiredHeightTexture() { return 0; }
	virtual void SetHeightTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** The texture is a (tangent space) normal-map.
     *
     *  Again, there are several conventions for tangent-space
     *  normal maps. Assimp does (intentionally) not
     *  distinguish here.
     */
	virtual u8 GetRequiredNormalsTexture() { return 0; }
	virtual void SetNormalsTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** The texture defines the glossiness of the material.
     *
     *  The glossiness is in fact the exponent of the specular
     *  (phong) lighting equation. Usually there is a conversion
     *  function defined to map the linear color values in the
     *  texture to a suitable exponent. Have fun.
    */
	virtual u8 GetRequiredShininessTexture() { return 0; }
	virtual void SetShininessTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** The texture defines per-pixel opacity.
     *
     *  Usually 'white' means opaque and 'black' means
     *  'transparency'. Or quite the opposite. Have fun.
    */
	virtual u8 GetRequiredOpacityTexture() { return 0; }
	virtual void SetOpacityTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** Displacement texture
     *
     *  The exact purpose and format is application-dependent.
     *  Higher color values stand for higher vertex displacements.
    */
	virtual u8 GetRequiredDisplacementTexture() { return 0; }
	virtual void SetDisplacementTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** Lightmap texture (aka Ambient Occlusion)
     *
     *  Both 'Lightmaps' and dedicated 'ambient occlusion maps' are
     *  covered by this material property. The texture contains a
     *  scaling value for the final color value of a pixel. Its
     *  intensity is not affected by incoming light.
    */
	virtual u8 GetRequiredLightmapTexture() { return 0; }
	virtual void SetLightmapTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** Reflection texture
     *
     * Contains the color of a perfect mirror reflection.
     * Rarely used, almost never for real-time applications.
    */
	virtual u8 GetRequiredReflectionTexture() { return 0; }
	virtual void SetReflectionTexture(sptr<GpuTexture> texture, u8 index) = 0;

    /** Unknown texture
     *
     *  A texture reference that does not match any of the definitions
     *  above is considered to be 'unknown'. It is still imported,
     *  but is excluded from any further postprocessing.
    */
	virtual u8 GetRequiredOtherTexture() { return 0; }
	virtual void SetOtherTexture(sptr<GpuTexture> texture, u8 index) = 0;
};

} // namespace RED_LILIUM_NAMESPACE
