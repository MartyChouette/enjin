# Next Steps: What to Build Next

## 🎯 Immediate Priorities (This Week)

### 1. Depth Buffer & Depth Testing 🔴 CRITICAL
**Why**: Essential for proper 3D rendering
**Time**: 1-2 days
**Impact**: Fixes z-fighting, enables proper 3D

**What to do**:
- Add depth attachment to render pass
- Create depth image/view
- Enable depth testing in pipeline

### 2. Complete Material System 🔴 CRITICAL  
**Why**: Everything needs materials
**Time**: 3-5 days
**Impact**: Flexible material system

**What to do**:
- Implement JSON parsing for materials
- Complete MaterialInstance::Load()
- Integrate with RenderSystem

### 3. Texture Loading 🔴 CRITICAL
**Why**: Materials need textures
**Time**: 2-3 days
**Impact**: Enables textured rendering

**What to do**:
- Integrate stb_image
- Create VulkanImage wrapper
- Add to bindless resources

## 🚀 Short Term (Next Month)

### 4. GPU Culling GPU Execution
- Fix compute shader execution
- Test performance vs CPU fallback

### 5. Render Graph System
- Implement dependency resolution
- Automatic barrier insertion

### 6. Complete Rendering Techniques
- Finish ForwardRendering
- Implement DeferredRendering

## 📚 Full Roadmap

See **[NEXT_FEATURES.md](docs/NEXT_FEATURES.md)** for complete prioritized roadmap.

---

**Start with Depth Buffer - it's quick and unlocks proper 3D rendering!**
