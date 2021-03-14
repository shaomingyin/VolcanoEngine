/*
 * This file was generated with gl3w_gen.py, part of gl3w
 * (hosted at https://github.com/skaslev/gl3w)
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef __vgl_h_
#define __vgl_h_

#include <GL/glcorearb.h>

#ifndef __gl_h_
#define __gl_h_
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define VGL_OK 0
#define VGL_ERROR_INIT -1
#define VGL_ERROR_LIBRARY_OPEN -2
#define VGL_ERROR_OPENGL_VERSION -3

union VGLProcs;

typedef void (*VGLProc)(void);
typedef VGLProc (*VGLGetProcAddressProc)(const char *procName);

/* vgl api */
int vglInit(union VGLProcs *procs, VGLGetProcAddressProc proc);
int vglIsSupported(int major, int minor);

/* vgl internal state */
union VGLProcs {
    VGLProc ptr[659];
    struct {
        PFNGLACTIVESHADERPROGRAMPROC                            ActiveShaderProgram;
        PFNGLACTIVETEXTUREPROC                                  ActiveTexture;
        PFNGLATTACHSHADERPROC                                   AttachShader;
        PFNGLBEGINCONDITIONALRENDERPROC                         BeginConditionalRender;
        PFNGLBEGINQUERYPROC                                     BeginQuery;
        PFNGLBEGINQUERYINDEXEDPROC                              BeginQueryIndexed;
        PFNGLBEGINTRANSFORMFEEDBACKPROC                         BeginTransformFeedback;
        PFNGLBINDATTRIBLOCATIONPROC                             BindAttribLocation;
        PFNGLBINDBUFFERPROC                                     BindBuffer;
        PFNGLBINDBUFFERBASEPROC                                 BindBufferBase;
        PFNGLBINDBUFFERRANGEPROC                                BindBufferRange;
        PFNGLBINDBUFFERSBASEPROC                                BindBuffersBase;
        PFNGLBINDBUFFERSRANGEPROC                               BindBuffersRange;
        PFNGLBINDFRAGDATALOCATIONPROC                           BindFragDataLocation;
        PFNGLBINDFRAGDATALOCATIONINDEXEDPROC                    BindFragDataLocationIndexed;
        PFNGLBINDFRAMEBUFFERPROC                                BindFramebuffer;
        PFNGLBINDIMAGETEXTUREPROC                               BindImageTexture;
        PFNGLBINDIMAGETEXTURESPROC                              BindImageTextures;
        PFNGLBINDPROGRAMPIPELINEPROC                            BindProgramPipeline;
        PFNGLBINDRENDERBUFFERPROC                               BindRenderbuffer;
        PFNGLBINDSAMPLERPROC                                    BindSampler;
        PFNGLBINDSAMPLERSPROC                                   BindSamplers;
        PFNGLBINDTEXTUREPROC                                    BindTexture;
        PFNGLBINDTEXTUREUNITPROC                                BindTextureUnit;
        PFNGLBINDTEXTURESPROC                                   BindTextures;
        PFNGLBINDTRANSFORMFEEDBACKPROC                          BindTransformFeedback;
        PFNGLBINDVERTEXARRAYPROC                                BindVertexArray;
        PFNGLBINDVERTEXBUFFERPROC                               BindVertexBuffer;
        PFNGLBINDVERTEXBUFFERSPROC                              BindVertexBuffers;
        PFNGLBLENDCOLORPROC                                     BlendColor;
        PFNGLBLENDEQUATIONPROC                                  BlendEquation;
        PFNGLBLENDEQUATIONSEPARATEPROC                          BlendEquationSeparate;
        PFNGLBLENDEQUATIONSEPARATEIPROC                         BlendEquationSeparatei;
        PFNGLBLENDEQUATIONIPROC                                 BlendEquationi;
        PFNGLBLENDFUNCPROC                                      BlendFunc;
        PFNGLBLENDFUNCSEPARATEPROC                              BlendFuncSeparate;
        PFNGLBLENDFUNCSEPARATEIPROC                             BlendFuncSeparatei;
        PFNGLBLENDFUNCIPROC                                     BlendFunci;
        PFNGLBLITFRAMEBUFFERPROC                                BlitFramebuffer;
        PFNGLBLITNAMEDFRAMEBUFFERPROC                           BlitNamedFramebuffer;
        PFNGLBUFFERDATAPROC                                     BufferData;
        PFNGLBUFFERSTORAGEPROC                                  BufferStorage;
        PFNGLBUFFERSUBDATAPROC                                  BufferSubData;
        PFNGLCHECKFRAMEBUFFERSTATUSPROC                         CheckFramebufferStatus;
        PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC                    CheckNamedFramebufferStatus;
        PFNGLCLAMPCOLORPROC                                     ClampColor;
        PFNGLCLEARPROC                                          Clear;
        PFNGLCLEARBUFFERDATAPROC                                ClearBufferData;
        PFNGLCLEARBUFFERSUBDATAPROC                             ClearBufferSubData;
        PFNGLCLEARBUFFERFIPROC                                  ClearBufferfi;
        PFNGLCLEARBUFFERFVPROC                                  ClearBufferfv;
        PFNGLCLEARBUFFERIVPROC                                  ClearBufferiv;
        PFNGLCLEARBUFFERUIVPROC                                 ClearBufferuiv;
        PFNGLCLEARCOLORPROC                                     ClearColor;
        PFNGLCLEARDEPTHPROC                                     ClearDepth;
        PFNGLCLEARDEPTHFPROC                                    ClearDepthf;
        PFNGLCLEARNAMEDBUFFERDATAPROC                           ClearNamedBufferData;
        PFNGLCLEARNAMEDBUFFERSUBDATAPROC                        ClearNamedBufferSubData;
        PFNGLCLEARNAMEDFRAMEBUFFERFIPROC                        ClearNamedFramebufferfi;
        PFNGLCLEARNAMEDFRAMEBUFFERFVPROC                        ClearNamedFramebufferfv;
        PFNGLCLEARNAMEDFRAMEBUFFERIVPROC                        ClearNamedFramebufferiv;
        PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC                       ClearNamedFramebufferuiv;
        PFNGLCLEARSTENCILPROC                                   ClearStencil;
        PFNGLCLEARTEXIMAGEPROC                                  ClearTexImage;
        PFNGLCLEARTEXSUBIMAGEPROC                               ClearTexSubImage;
        PFNGLCLIENTWAITSYNCPROC                                 ClientWaitSync;
        PFNGLCLIPCONTROLPROC                                    ClipControl;
        PFNGLCOLORMASKPROC                                      ColorMask;
        PFNGLCOLORMASKIPROC                                     ColorMaski;
        PFNGLCOMPILESHADERPROC                                  CompileShader;
        PFNGLCOMPRESSEDTEXIMAGE1DPROC                           CompressedTexImage1D;
        PFNGLCOMPRESSEDTEXIMAGE2DPROC                           CompressedTexImage2D;
        PFNGLCOMPRESSEDTEXIMAGE3DPROC                           CompressedTexImage3D;
        PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC                        CompressedTexSubImage1D;
        PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC                        CompressedTexSubImage2D;
        PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC                        CompressedTexSubImage3D;
        PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC                    CompressedTextureSubImage1D;
        PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC                    CompressedTextureSubImage2D;
        PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC                    CompressedTextureSubImage3D;
        PFNGLCOPYBUFFERSUBDATAPROC                              CopyBufferSubData;
        PFNGLCOPYIMAGESUBDATAPROC                               CopyImageSubData;
        PFNGLCOPYNAMEDBUFFERSUBDATAPROC                         CopyNamedBufferSubData;
        PFNGLCOPYTEXIMAGE1DPROC                                 CopyTexImage1D;
        PFNGLCOPYTEXIMAGE2DPROC                                 CopyTexImage2D;
        PFNGLCOPYTEXSUBIMAGE1DPROC                              CopyTexSubImage1D;
        PFNGLCOPYTEXSUBIMAGE2DPROC                              CopyTexSubImage2D;
        PFNGLCOPYTEXSUBIMAGE3DPROC                              CopyTexSubImage3D;
        PFNGLCOPYTEXTURESUBIMAGE1DPROC                          CopyTextureSubImage1D;
        PFNGLCOPYTEXTURESUBIMAGE2DPROC                          CopyTextureSubImage2D;
        PFNGLCOPYTEXTURESUBIMAGE3DPROC                          CopyTextureSubImage3D;
        PFNGLCREATEBUFFERSPROC                                  CreateBuffers;
        PFNGLCREATEFRAMEBUFFERSPROC                             CreateFramebuffers;
        PFNGLCREATEPROGRAMPROC                                  CreateProgram;
        PFNGLCREATEPROGRAMPIPELINESPROC                         CreateProgramPipelines;
        PFNGLCREATEQUERIESPROC                                  CreateQueries;
        PFNGLCREATERENDERBUFFERSPROC                            CreateRenderbuffers;
        PFNGLCREATESAMPLERSPROC                                 CreateSamplers;
        PFNGLCREATESHADERPROC                                   CreateShader;
        PFNGLCREATESHADERPROGRAMVPROC                           CreateShaderProgramv;
        PFNGLCREATETEXTURESPROC                                 CreateTextures;
        PFNGLCREATETRANSFORMFEEDBACKSPROC                       CreateTransformFeedbacks;
        PFNGLCREATEVERTEXARRAYSPROC                             CreateVertexArrays;
        PFNGLCULLFACEPROC                                       CullFace;
        PFNGLDEBUGMESSAGECALLBACKPROC                           DebugMessageCallback;
        PFNGLDEBUGMESSAGECONTROLPROC                            DebugMessageControl;
        PFNGLDEBUGMESSAGEINSERTPROC                             DebugMessageInsert;
        PFNGLDELETEBUFFERSPROC                                  DeleteBuffers;
        PFNGLDELETEFRAMEBUFFERSPROC                             DeleteFramebuffers;
        PFNGLDELETEPROGRAMPROC                                  DeleteProgram;
        PFNGLDELETEPROGRAMPIPELINESPROC                         DeleteProgramPipelines;
        PFNGLDELETEQUERIESPROC                                  DeleteQueries;
        PFNGLDELETERENDERBUFFERSPROC                            DeleteRenderbuffers;
        PFNGLDELETESAMPLERSPROC                                 DeleteSamplers;
        PFNGLDELETESHADERPROC                                   DeleteShader;
        PFNGLDELETESYNCPROC                                     DeleteSync;
        PFNGLDELETETEXTURESPROC                                 DeleteTextures;
        PFNGLDELETETRANSFORMFEEDBACKSPROC                       DeleteTransformFeedbacks;
        PFNGLDELETEVERTEXARRAYSPROC                             DeleteVertexArrays;
        PFNGLDEPTHFUNCPROC                                      DepthFunc;
        PFNGLDEPTHMASKPROC                                      DepthMask;
        PFNGLDEPTHRANGEPROC                                     DepthRange;
        PFNGLDEPTHRANGEARRAYVPROC                               DepthRangeArrayv;
        PFNGLDEPTHRANGEINDEXEDPROC                              DepthRangeIndexed;
        PFNGLDEPTHRANGEFPROC                                    DepthRangef;
        PFNGLDETACHSHADERPROC                                   DetachShader;
        PFNGLDISABLEPROC                                        Disable;
        PFNGLDISABLEVERTEXARRAYATTRIBPROC                       DisableVertexArrayAttrib;
        PFNGLDISABLEVERTEXATTRIBARRAYPROC                       DisableVertexAttribArray;
        PFNGLDISABLEIPROC                                       Disablei;
        PFNGLDISPATCHCOMPUTEPROC                                DispatchCompute;
        PFNGLDISPATCHCOMPUTEINDIRECTPROC                        DispatchComputeIndirect;
        PFNGLDRAWARRAYSPROC                                     DrawArrays;
        PFNGLDRAWARRAYSINDIRECTPROC                             DrawArraysIndirect;
        PFNGLDRAWARRAYSINSTANCEDPROC                            DrawArraysInstanced;
        PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC                DrawArraysInstancedBaseInstance;
        PFNGLDRAWBUFFERPROC                                     DrawBuffer;
        PFNGLDRAWBUFFERSPROC                                    DrawBuffers;
        PFNGLDRAWELEMENTSPROC                                   DrawElements;
        PFNGLDRAWELEMENTSBASEVERTEXPROC                         DrawElementsBaseVertex;
        PFNGLDRAWELEMENTSINDIRECTPROC                           DrawElementsIndirect;
        PFNGLDRAWELEMENTSINSTANCEDPROC                          DrawElementsInstanced;
        PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC              DrawElementsInstancedBaseInstance;
        PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC                DrawElementsInstancedBaseVertex;
        PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC    DrawElementsInstancedBaseVertexBaseInstance;
        PFNGLDRAWRANGEELEMENTSPROC                              DrawRangeElements;
        PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC                    DrawRangeElementsBaseVertex;
        PFNGLDRAWTRANSFORMFEEDBACKPROC                          DrawTransformFeedback;
        PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC                 DrawTransformFeedbackInstanced;
        PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC                    DrawTransformFeedbackStream;
        PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC           DrawTransformFeedbackStreamInstanced;
        PFNGLENABLEPROC                                         Enable;
        PFNGLENABLEVERTEXARRAYATTRIBPROC                        EnableVertexArrayAttrib;
        PFNGLENABLEVERTEXATTRIBARRAYPROC                        EnableVertexAttribArray;
        PFNGLENABLEIPROC                                        Enablei;
        PFNGLENDCONDITIONALRENDERPROC                           EndConditionalRender;
        PFNGLENDQUERYPROC                                       EndQuery;
        PFNGLENDQUERYINDEXEDPROC                                EndQueryIndexed;
        PFNGLENDTRANSFORMFEEDBACKPROC                           EndTransformFeedback;
        PFNGLFENCESYNCPROC                                      FenceSync;
        PFNGLFINISHPROC                                         Finish;
        PFNGLFLUSHPROC                                          Flush;
        PFNGLFLUSHMAPPEDBUFFERRANGEPROC                         FlushMappedBufferRange;
        PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC                    FlushMappedNamedBufferRange;
        PFNGLFRAMEBUFFERPARAMETERIPROC                          FramebufferParameteri;
        PFNGLFRAMEBUFFERPARAMETERIMESAPROC                      FramebufferParameteriMESA;
        PFNGLFRAMEBUFFERRENDERBUFFERPROC                        FramebufferRenderbuffer;
        PFNGLFRAMEBUFFERTEXTUREPROC                             FramebufferTexture;
        PFNGLFRAMEBUFFERTEXTURE1DPROC                           FramebufferTexture1D;
        PFNGLFRAMEBUFFERTEXTURE2DPROC                           FramebufferTexture2D;
        PFNGLFRAMEBUFFERTEXTURE3DPROC                           FramebufferTexture3D;
        PFNGLFRAMEBUFFERTEXTURELAYERPROC                        FramebufferTextureLayer;
        PFNGLFRONTFACEPROC                                      FrontFace;
        PFNGLGENBUFFERSPROC                                     GenBuffers;
        PFNGLGENFRAMEBUFFERSPROC                                GenFramebuffers;
        PFNGLGENPROGRAMPIPELINESPROC                            GenProgramPipelines;
        PFNGLGENQUERIESPROC                                     GenQueries;
        PFNGLGENRENDERBUFFERSPROC                               GenRenderbuffers;
        PFNGLGENSAMPLERSPROC                                    GenSamplers;
        PFNGLGENTEXTURESPROC                                    GenTextures;
        PFNGLGENTRANSFORMFEEDBACKSPROC                          GenTransformFeedbacks;
        PFNGLGENVERTEXARRAYSPROC                                GenVertexArrays;
        PFNGLGENERATEMIPMAPPROC                                 GenerateMipmap;
        PFNGLGENERATETEXTUREMIPMAPPROC                          GenerateTextureMipmap;
        PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC                 GetActiveAtomicCounterBufferiv;
        PFNGLGETACTIVEATTRIBPROC                                GetActiveAttrib;
        PFNGLGETACTIVESUBROUTINENAMEPROC                        GetActiveSubroutineName;
        PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC                 GetActiveSubroutineUniformName;
        PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC                   GetActiveSubroutineUniformiv;
        PFNGLGETACTIVEUNIFORMPROC                               GetActiveUniform;
        PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC                      GetActiveUniformBlockName;
        PFNGLGETACTIVEUNIFORMBLOCKIVPROC                        GetActiveUniformBlockiv;
        PFNGLGETACTIVEUNIFORMNAMEPROC                           GetActiveUniformName;
        PFNGLGETACTIVEUNIFORMSIVPROC                            GetActiveUniformsiv;
        PFNGLGETATTACHEDSHADERSPROC                             GetAttachedShaders;
        PFNGLGETATTRIBLOCATIONPROC                              GetAttribLocation;
        PFNGLGETBOOLEANI_VPROC                                  GetBooleani_v;
        PFNGLGETBOOLEANVPROC                                    GetBooleanv;
        PFNGLGETBUFFERPARAMETERI64VPROC                         GetBufferParameteri64v;
        PFNGLGETBUFFERPARAMETERIVPROC                           GetBufferParameteriv;
        PFNGLGETBUFFERPOINTERVPROC                              GetBufferPointerv;
        PFNGLGETBUFFERSUBDATAPROC                               GetBufferSubData;
        PFNGLGETCOMPRESSEDTEXIMAGEPROC                          GetCompressedTexImage;
        PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC                      GetCompressedTextureImage;
        PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC                   GetCompressedTextureSubImage;
        PFNGLGETDEBUGMESSAGELOGPROC                             GetDebugMessageLog;
        PFNGLGETDOUBLEI_VPROC                                   GetDoublei_v;
        PFNGLGETDOUBLEVPROC                                     GetDoublev;
        PFNGLGETERRORPROC                                       GetError;
        PFNGLGETFLOATI_VPROC                                    GetFloati_v;
        PFNGLGETFLOATVPROC                                      GetFloatv;
        PFNGLGETFRAGDATAINDEXPROC                               GetFragDataIndex;
        PFNGLGETFRAGDATALOCATIONPROC                            GetFragDataLocation;
        PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC            GetFramebufferAttachmentParameteriv;
        PFNGLGETFRAMEBUFFERPARAMETERIVPROC                      GetFramebufferParameteriv;
        PFNGLGETFRAMEBUFFERPARAMETERIVMESAPROC                  GetFramebufferParameterivMESA;
        PFNGLGETGRAPHICSRESETSTATUSPROC                         GetGraphicsResetStatus;
        PFNGLGETINTEGER64I_VPROC                                GetInteger64i_v;
        PFNGLGETINTEGER64VPROC                                  GetInteger64v;
        PFNGLGETINTEGERI_VPROC                                  GetIntegeri_v;
        PFNGLGETINTEGERVPROC                                    GetIntegerv;
        PFNGLGETINTERNALFORMATI64VPROC                          GetInternalformati64v;
        PFNGLGETINTERNALFORMATIVPROC                            GetInternalformativ;
        PFNGLGETMULTISAMPLEFVPROC                               GetMultisamplefv;
        PFNGLGETNAMEDBUFFERPARAMETERI64VPROC                    GetNamedBufferParameteri64v;
        PFNGLGETNAMEDBUFFERPARAMETERIVPROC                      GetNamedBufferParameteriv;
        PFNGLGETNAMEDBUFFERPOINTERVPROC                         GetNamedBufferPointerv;
        PFNGLGETNAMEDBUFFERSUBDATAPROC                          GetNamedBufferSubData;
        PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC       GetNamedFramebufferAttachmentParameteriv;
        PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC                 GetNamedFramebufferParameteriv;
        PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC                GetNamedRenderbufferParameteriv;
        PFNGLGETOBJECTLABELPROC                                 GetObjectLabel;
        PFNGLGETOBJECTPTRLABELPROC                              GetObjectPtrLabel;
        PFNGLGETPOINTERVPROC                                    GetPointerv;
        PFNGLGETPROGRAMBINARYPROC                               GetProgramBinary;
        PFNGLGETPROGRAMINFOLOGPROC                              GetProgramInfoLog;
        PFNGLGETPROGRAMINTERFACEIVPROC                          GetProgramInterfaceiv;
        PFNGLGETPROGRAMPIPELINEINFOLOGPROC                      GetProgramPipelineInfoLog;
        PFNGLGETPROGRAMPIPELINEIVPROC                           GetProgramPipelineiv;
        PFNGLGETPROGRAMRESOURCEINDEXPROC                        GetProgramResourceIndex;
        PFNGLGETPROGRAMRESOURCELOCATIONPROC                     GetProgramResourceLocation;
        PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC                GetProgramResourceLocationIndex;
        PFNGLGETPROGRAMRESOURCENAMEPROC                         GetProgramResourceName;
        PFNGLGETPROGRAMRESOURCEIVPROC                           GetProgramResourceiv;
        PFNGLGETPROGRAMSTAGEIVPROC                              GetProgramStageiv;
        PFNGLGETPROGRAMIVPROC                                   GetProgramiv;
        PFNGLGETQUERYBUFFEROBJECTI64VPROC                       GetQueryBufferObjecti64v;
        PFNGLGETQUERYBUFFEROBJECTIVPROC                         GetQueryBufferObjectiv;
        PFNGLGETQUERYBUFFEROBJECTUI64VPROC                      GetQueryBufferObjectui64v;
        PFNGLGETQUERYBUFFEROBJECTUIVPROC                        GetQueryBufferObjectuiv;
        PFNGLGETQUERYINDEXEDIVPROC                              GetQueryIndexediv;
        PFNGLGETQUERYOBJECTI64VPROC                             GetQueryObjecti64v;
        PFNGLGETQUERYOBJECTIVPROC                               GetQueryObjectiv;
        PFNGLGETQUERYOBJECTUI64VPROC                            GetQueryObjectui64v;
        PFNGLGETQUERYOBJECTUIVPROC                              GetQueryObjectuiv;
        PFNGLGETQUERYIVPROC                                     GetQueryiv;
        PFNGLGETRENDERBUFFERPARAMETERIVPROC                     GetRenderbufferParameteriv;
        PFNGLGETSAMPLERPARAMETERIIVPROC                         GetSamplerParameterIiv;
        PFNGLGETSAMPLERPARAMETERIUIVPROC                        GetSamplerParameterIuiv;
        PFNGLGETSAMPLERPARAMETERFVPROC                          GetSamplerParameterfv;
        PFNGLGETSAMPLERPARAMETERIVPROC                          GetSamplerParameteriv;
        PFNGLGETSHADERINFOLOGPROC                               GetShaderInfoLog;
        PFNGLGETSHADERPRECISIONFORMATPROC                       GetShaderPrecisionFormat;
        PFNGLGETSHADERSOURCEPROC                                GetShaderSource;
        PFNGLGETSHADERIVPROC                                    GetShaderiv;
        PFNGLGETSTRINGPROC                                      GetString;
        PFNGLGETSTRINGIPROC                                     GetStringi;
        PFNGLGETSUBROUTINEINDEXPROC                             GetSubroutineIndex;
        PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC                   GetSubroutineUniformLocation;
        PFNGLGETSYNCIVPROC                                      GetSynciv;
        PFNGLGETTEXIMAGEPROC                                    GetTexImage;
        PFNGLGETTEXLEVELPARAMETERFVPROC                         GetTexLevelParameterfv;
        PFNGLGETTEXLEVELPARAMETERIVPROC                         GetTexLevelParameteriv;
        PFNGLGETTEXPARAMETERIIVPROC                             GetTexParameterIiv;
        PFNGLGETTEXPARAMETERIUIVPROC                            GetTexParameterIuiv;
        PFNGLGETTEXPARAMETERFVPROC                              GetTexParameterfv;
        PFNGLGETTEXPARAMETERIVPROC                              GetTexParameteriv;
        PFNGLGETTEXTUREIMAGEPROC                                GetTextureImage;
        PFNGLGETTEXTURELEVELPARAMETERFVPROC                     GetTextureLevelParameterfv;
        PFNGLGETTEXTURELEVELPARAMETERIVPROC                     GetTextureLevelParameteriv;
        PFNGLGETTEXTUREPARAMETERIIVPROC                         GetTextureParameterIiv;
        PFNGLGETTEXTUREPARAMETERIUIVPROC                        GetTextureParameterIuiv;
        PFNGLGETTEXTUREPARAMETERFVPROC                          GetTextureParameterfv;
        PFNGLGETTEXTUREPARAMETERIVPROC                          GetTextureParameteriv;
        PFNGLGETTEXTURESUBIMAGEPROC                             GetTextureSubImage;
        PFNGLGETTRANSFORMFEEDBACKVARYINGPROC                    GetTransformFeedbackVarying;
        PFNGLGETTRANSFORMFEEDBACKI64_VPROC                      GetTransformFeedbacki64_v;
        PFNGLGETTRANSFORMFEEDBACKI_VPROC                        GetTransformFeedbacki_v;
        PFNGLGETTRANSFORMFEEDBACKIVPROC                         GetTransformFeedbackiv;
        PFNGLGETUNIFORMBLOCKINDEXPROC                           GetUniformBlockIndex;
        PFNGLGETUNIFORMINDICESPROC                              GetUniformIndices;
        PFNGLGETUNIFORMLOCATIONPROC                             GetUniformLocation;
        PFNGLGETUNIFORMSUBROUTINEUIVPROC                        GetUniformSubroutineuiv;
        PFNGLGETUNIFORMDVPROC                                   GetUniformdv;
        PFNGLGETUNIFORMFVPROC                                   GetUniformfv;
        PFNGLGETUNIFORMIVPROC                                   GetUniformiv;
        PFNGLGETUNIFORMUIVPROC                                  GetUniformuiv;
        PFNGLGETVERTEXARRAYINDEXED64IVPROC                      GetVertexArrayIndexed64iv;
        PFNGLGETVERTEXARRAYINDEXEDIVPROC                        GetVertexArrayIndexediv;
        PFNGLGETVERTEXARRAYIVPROC                               GetVertexArrayiv;
        PFNGLGETVERTEXATTRIBIIVPROC                             GetVertexAttribIiv;
        PFNGLGETVERTEXATTRIBIUIVPROC                            GetVertexAttribIuiv;
        PFNGLGETVERTEXATTRIBLDVPROC                             GetVertexAttribLdv;
        PFNGLGETVERTEXATTRIBPOINTERVPROC                        GetVertexAttribPointerv;
        PFNGLGETVERTEXATTRIBDVPROC                              GetVertexAttribdv;
        PFNGLGETVERTEXATTRIBFVPROC                              GetVertexAttribfv;
        PFNGLGETVERTEXATTRIBIVPROC                              GetVertexAttribiv;
        PFNGLGETNCOMPRESSEDTEXIMAGEPROC                         GetnCompressedTexImage;
        PFNGLGETNTEXIMAGEPROC                                   GetnTexImage;
        PFNGLGETNUNIFORMDVPROC                                  GetnUniformdv;
        PFNGLGETNUNIFORMFVPROC                                  GetnUniformfv;
        PFNGLGETNUNIFORMIVPROC                                  GetnUniformiv;
        PFNGLGETNUNIFORMUIVPROC                                 GetnUniformuiv;
        PFNGLHINTPROC                                           Hint;
        PFNGLINVALIDATEBUFFERDATAPROC                           InvalidateBufferData;
        PFNGLINVALIDATEBUFFERSUBDATAPROC                        InvalidateBufferSubData;
        PFNGLINVALIDATEFRAMEBUFFERPROC                          InvalidateFramebuffer;
        PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC                 InvalidateNamedFramebufferData;
        PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC              InvalidateNamedFramebufferSubData;
        PFNGLINVALIDATESUBFRAMEBUFFERPROC                       InvalidateSubFramebuffer;
        PFNGLINVALIDATETEXIMAGEPROC                             InvalidateTexImage;
        PFNGLINVALIDATETEXSUBIMAGEPROC                          InvalidateTexSubImage;
        PFNGLISBUFFERPROC                                       IsBuffer;
        PFNGLISENABLEDPROC                                      IsEnabled;
        PFNGLISENABLEDIPROC                                     IsEnabledi;
        PFNGLISFRAMEBUFFERPROC                                  IsFramebuffer;
        PFNGLISPROGRAMPROC                                      IsProgram;
        PFNGLISPROGRAMPIPELINEPROC                              IsProgramPipeline;
        PFNGLISQUERYPROC                                        IsQuery;
        PFNGLISRENDERBUFFERPROC                                 IsRenderbuffer;
        PFNGLISSAMPLERPROC                                      IsSampler;
        PFNGLISSHADERPROC                                       IsShader;
        PFNGLISSYNCPROC                                         IsSync;
        PFNGLISTEXTUREPROC                                      IsTexture;
        PFNGLISTRANSFORMFEEDBACKPROC                            IsTransformFeedback;
        PFNGLISVERTEXARRAYPROC                                  IsVertexArray;
        PFNGLLINEWIDTHPROC                                      LineWidth;
        PFNGLLINKPROGRAMPROC                                    LinkProgram;
        PFNGLLOGICOPPROC                                        LogicOp;
        PFNGLMAPBUFFERPROC                                      MapBuffer;
        PFNGLMAPBUFFERRANGEPROC                                 MapBufferRange;
        PFNGLMAPNAMEDBUFFERPROC                                 MapNamedBuffer;
        PFNGLMAPNAMEDBUFFERRANGEPROC                            MapNamedBufferRange;
        PFNGLMEMORYBARRIERPROC                                  MemoryBarrier;
        PFNGLMEMORYBARRIERBYREGIONPROC                          MemoryBarrierByRegion;
        PFNGLMINSAMPLESHADINGPROC                               MinSampleShading;
        PFNGLMULTIDRAWARRAYSPROC                                MultiDrawArrays;
        PFNGLMULTIDRAWARRAYSINDIRECTPROC                        MultiDrawArraysIndirect;
        PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC                   MultiDrawArraysIndirectCount;
        PFNGLMULTIDRAWELEMENTSPROC                              MultiDrawElements;
        PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC                    MultiDrawElementsBaseVertex;
        PFNGLMULTIDRAWELEMENTSINDIRECTPROC                      MultiDrawElementsIndirect;
        PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC                 MultiDrawElementsIndirectCount;
        PFNGLNAMEDBUFFERDATAPROC                                NamedBufferData;
        PFNGLNAMEDBUFFERSTORAGEPROC                             NamedBufferStorage;
        PFNGLNAMEDBUFFERSUBDATAPROC                             NamedBufferSubData;
        PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC                     NamedFramebufferDrawBuffer;
        PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC                    NamedFramebufferDrawBuffers;
        PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC                     NamedFramebufferParameteri;
        PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC                     NamedFramebufferReadBuffer;
        PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC                   NamedFramebufferRenderbuffer;
        PFNGLNAMEDFRAMEBUFFERTEXTUREPROC                        NamedFramebufferTexture;
        PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC                   NamedFramebufferTextureLayer;
        PFNGLNAMEDRENDERBUFFERSTORAGEPROC                       NamedRenderbufferStorage;
        PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC            NamedRenderbufferStorageMultisample;
        PFNGLOBJECTLABELPROC                                    ObjectLabel;
        PFNGLOBJECTPTRLABELPROC                                 ObjectPtrLabel;
        PFNGLPATCHPARAMETERFVPROC                               PatchParameterfv;
        PFNGLPATCHPARAMETERIPROC                                PatchParameteri;
        PFNGLPAUSETRANSFORMFEEDBACKPROC                         PauseTransformFeedback;
        PFNGLPIXELSTOREFPROC                                    PixelStoref;
        PFNGLPIXELSTOREIPROC                                    PixelStorei;
        PFNGLPOINTPARAMETERFPROC                                PointParameterf;
        PFNGLPOINTPARAMETERFVPROC                               PointParameterfv;
        PFNGLPOINTPARAMETERIPROC                                PointParameteri;
        PFNGLPOINTPARAMETERIVPROC                               PointParameteriv;
        PFNGLPOINTSIZEPROC                                      PointSize;
        PFNGLPOLYGONMODEPROC                                    PolygonMode;
        PFNGLPOLYGONOFFSETPROC                                  PolygonOffset;
        PFNGLPOLYGONOFFSETCLAMPPROC                             PolygonOffsetClamp;
        PFNGLPOPDEBUGGROUPPROC                                  PopDebugGroup;
        PFNGLPRIMITIVERESTARTINDEXPROC                          PrimitiveRestartIndex;
        PFNGLPROGRAMBINARYPROC                                  ProgramBinary;
        PFNGLPROGRAMPARAMETERIPROC                              ProgramParameteri;
        PFNGLPROGRAMUNIFORM1DPROC                               ProgramUniform1d;
        PFNGLPROGRAMUNIFORM1DVPROC                              ProgramUniform1dv;
        PFNGLPROGRAMUNIFORM1FPROC                               ProgramUniform1f;
        PFNGLPROGRAMUNIFORM1FVPROC                              ProgramUniform1fv;
        PFNGLPROGRAMUNIFORM1IPROC                               ProgramUniform1i;
        PFNGLPROGRAMUNIFORM1IVPROC                              ProgramUniform1iv;
        PFNGLPROGRAMUNIFORM1UIPROC                              ProgramUniform1ui;
        PFNGLPROGRAMUNIFORM1UIVPROC                             ProgramUniform1uiv;
        PFNGLPROGRAMUNIFORM2DPROC                               ProgramUniform2d;
        PFNGLPROGRAMUNIFORM2DVPROC                              ProgramUniform2dv;
        PFNGLPROGRAMUNIFORM2FPROC                               ProgramUniform2f;
        PFNGLPROGRAMUNIFORM2FVPROC                              ProgramUniform2fv;
        PFNGLPROGRAMUNIFORM2IPROC                               ProgramUniform2i;
        PFNGLPROGRAMUNIFORM2IVPROC                              ProgramUniform2iv;
        PFNGLPROGRAMUNIFORM2UIPROC                              ProgramUniform2ui;
        PFNGLPROGRAMUNIFORM2UIVPROC                             ProgramUniform2uiv;
        PFNGLPROGRAMUNIFORM3DPROC                               ProgramUniform3d;
        PFNGLPROGRAMUNIFORM3DVPROC                              ProgramUniform3dv;
        PFNGLPROGRAMUNIFORM3FPROC                               ProgramUniform3f;
        PFNGLPROGRAMUNIFORM3FVPROC                              ProgramUniform3fv;
        PFNGLPROGRAMUNIFORM3IPROC                               ProgramUniform3i;
        PFNGLPROGRAMUNIFORM3IVPROC                              ProgramUniform3iv;
        PFNGLPROGRAMUNIFORM3UIPROC                              ProgramUniform3ui;
        PFNGLPROGRAMUNIFORM3UIVPROC                             ProgramUniform3uiv;
        PFNGLPROGRAMUNIFORM4DPROC                               ProgramUniform4d;
        PFNGLPROGRAMUNIFORM4DVPROC                              ProgramUniform4dv;
        PFNGLPROGRAMUNIFORM4FPROC                               ProgramUniform4f;
        PFNGLPROGRAMUNIFORM4FVPROC                              ProgramUniform4fv;
        PFNGLPROGRAMUNIFORM4IPROC                               ProgramUniform4i;
        PFNGLPROGRAMUNIFORM4IVPROC                              ProgramUniform4iv;
        PFNGLPROGRAMUNIFORM4UIPROC                              ProgramUniform4ui;
        PFNGLPROGRAMUNIFORM4UIVPROC                             ProgramUniform4uiv;
        PFNGLPROGRAMUNIFORMMATRIX2DVPROC                        ProgramUniformMatrix2dv;
        PFNGLPROGRAMUNIFORMMATRIX2FVPROC                        ProgramUniformMatrix2fv;
        PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC                      ProgramUniformMatrix2x3dv;
        PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC                      ProgramUniformMatrix2x3fv;
        PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC                      ProgramUniformMatrix2x4dv;
        PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC                      ProgramUniformMatrix2x4fv;
        PFNGLPROGRAMUNIFORMMATRIX3DVPROC                        ProgramUniformMatrix3dv;
        PFNGLPROGRAMUNIFORMMATRIX3FVPROC                        ProgramUniformMatrix3fv;
        PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC                      ProgramUniformMatrix3x2dv;
        PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC                      ProgramUniformMatrix3x2fv;
        PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC                      ProgramUniformMatrix3x4dv;
        PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC                      ProgramUniformMatrix3x4fv;
        PFNGLPROGRAMUNIFORMMATRIX4DVPROC                        ProgramUniformMatrix4dv;
        PFNGLPROGRAMUNIFORMMATRIX4FVPROC                        ProgramUniformMatrix4fv;
        PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC                      ProgramUniformMatrix4x2dv;
        PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC                      ProgramUniformMatrix4x2fv;
        PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC                      ProgramUniformMatrix4x3dv;
        PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC                      ProgramUniformMatrix4x3fv;
        PFNGLPROVOKINGVERTEXPROC                                ProvokingVertex;
        PFNGLPUSHDEBUGGROUPPROC                                 PushDebugGroup;
        PFNGLQUERYCOUNTERPROC                                   QueryCounter;
        PFNGLREADBUFFERPROC                                     ReadBuffer;
        PFNGLREADPIXELSPROC                                     ReadPixels;
        PFNGLREADNPIXELSPROC                                    ReadnPixels;
        PFNGLRELEASESHADERCOMPILERPROC                          ReleaseShaderCompiler;
        PFNGLRENDERBUFFERSTORAGEPROC                            RenderbufferStorage;
        PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC                 RenderbufferStorageMultisample;
        PFNGLRESUMETRANSFORMFEEDBACKPROC                        ResumeTransformFeedback;
        PFNGLSAMPLECOVERAGEPROC                                 SampleCoverage;
        PFNGLSAMPLEMASKIPROC                                    SampleMaski;
        PFNGLSAMPLERPARAMETERIIVPROC                            SamplerParameterIiv;
        PFNGLSAMPLERPARAMETERIUIVPROC                           SamplerParameterIuiv;
        PFNGLSAMPLERPARAMETERFPROC                              SamplerParameterf;
        PFNGLSAMPLERPARAMETERFVPROC                             SamplerParameterfv;
        PFNGLSAMPLERPARAMETERIPROC                              SamplerParameteri;
        PFNGLSAMPLERPARAMETERIVPROC                             SamplerParameteriv;
        PFNGLSCISSORPROC                                        Scissor;
        PFNGLSCISSORARRAYVPROC                                  ScissorArrayv;
        PFNGLSCISSORINDEXEDPROC                                 ScissorIndexed;
        PFNGLSCISSORINDEXEDVPROC                                ScissorIndexedv;
        PFNGLSHADERBINARYPROC                                   ShaderBinary;
        PFNGLSHADERSOURCEPROC                                   ShaderSource;
        PFNGLSHADERSTORAGEBLOCKBINDINGPROC                      ShaderStorageBlockBinding;
        PFNGLSPECIALIZESHADERPROC                               SpecializeShader;
        PFNGLSTENCILFUNCPROC                                    StencilFunc;
        PFNGLSTENCILFUNCSEPARATEPROC                            StencilFuncSeparate;
        PFNGLSTENCILMASKPROC                                    StencilMask;
        PFNGLSTENCILMASKSEPARATEPROC                            StencilMaskSeparate;
        PFNGLSTENCILOPPROC                                      StencilOp;
        PFNGLSTENCILOPSEPARATEPROC                              StencilOpSeparate;
        PFNGLTEXBUFFERPROC                                      TexBuffer;
        PFNGLTEXBUFFERRANGEPROC                                 TexBufferRange;
        PFNGLTEXIMAGE1DPROC                                     TexImage1D;
        PFNGLTEXIMAGE2DPROC                                     TexImage2D;
        PFNGLTEXIMAGE2DMULTISAMPLEPROC                          TexImage2DMultisample;
        PFNGLTEXIMAGE3DPROC                                     TexImage3D;
        PFNGLTEXIMAGE3DMULTISAMPLEPROC                          TexImage3DMultisample;
        PFNGLTEXPARAMETERIIVPROC                                TexParameterIiv;
        PFNGLTEXPARAMETERIUIVPROC                               TexParameterIuiv;
        PFNGLTEXPARAMETERFPROC                                  TexParameterf;
        PFNGLTEXPARAMETERFVPROC                                 TexParameterfv;
        PFNGLTEXPARAMETERIPROC                                  TexParameteri;
        PFNGLTEXPARAMETERIVPROC                                 TexParameteriv;
        PFNGLTEXSTORAGE1DPROC                                   TexStorage1D;
        PFNGLTEXSTORAGE2DPROC                                   TexStorage2D;
        PFNGLTEXSTORAGE2DMULTISAMPLEPROC                        TexStorage2DMultisample;
        PFNGLTEXSTORAGE3DPROC                                   TexStorage3D;
        PFNGLTEXSTORAGE3DMULTISAMPLEPROC                        TexStorage3DMultisample;
        PFNGLTEXSUBIMAGE1DPROC                                  TexSubImage1D;
        PFNGLTEXSUBIMAGE2DPROC                                  TexSubImage2D;
        PFNGLTEXSUBIMAGE3DPROC                                  TexSubImage3D;
        PFNGLTEXTUREBARRIERPROC                                 TextureBarrier;
        PFNGLTEXTUREBUFFERPROC                                  TextureBuffer;
        PFNGLTEXTUREBUFFERRANGEPROC                             TextureBufferRange;
        PFNGLTEXTUREPARAMETERIIVPROC                            TextureParameterIiv;
        PFNGLTEXTUREPARAMETERIUIVPROC                           TextureParameterIuiv;
        PFNGLTEXTUREPARAMETERFPROC                              TextureParameterf;
        PFNGLTEXTUREPARAMETERFVPROC                             TextureParameterfv;
        PFNGLTEXTUREPARAMETERIPROC                              TextureParameteri;
        PFNGLTEXTUREPARAMETERIVPROC                             TextureParameteriv;
        PFNGLTEXTURESTORAGE1DPROC                               TextureStorage1D;
        PFNGLTEXTURESTORAGE2DPROC                               TextureStorage2D;
        PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC                    TextureStorage2DMultisample;
        PFNGLTEXTURESTORAGE3DPROC                               TextureStorage3D;
        PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC                    TextureStorage3DMultisample;
        PFNGLTEXTURESUBIMAGE1DPROC                              TextureSubImage1D;
        PFNGLTEXTURESUBIMAGE2DPROC                              TextureSubImage2D;
        PFNGLTEXTURESUBIMAGE3DPROC                              TextureSubImage3D;
        PFNGLTEXTUREVIEWPROC                                    TextureView;
        PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC                    TransformFeedbackBufferBase;
        PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC                   TransformFeedbackBufferRange;
        PFNGLTRANSFORMFEEDBACKVARYINGSPROC                      TransformFeedbackVaryings;
        PFNGLUNIFORM1DPROC                                      Uniform1d;
        PFNGLUNIFORM1DVPROC                                     Uniform1dv;
        PFNGLUNIFORM1FPROC                                      Uniform1f;
        PFNGLUNIFORM1FVPROC                                     Uniform1fv;
        PFNGLUNIFORM1IPROC                                      Uniform1i;
        PFNGLUNIFORM1IVPROC                                     Uniform1iv;
        PFNGLUNIFORM1UIPROC                                     Uniform1ui;
        PFNGLUNIFORM1UIVPROC                                    Uniform1uiv;
        PFNGLUNIFORM2DPROC                                      Uniform2d;
        PFNGLUNIFORM2DVPROC                                     Uniform2dv;
        PFNGLUNIFORM2FPROC                                      Uniform2f;
        PFNGLUNIFORM2FVPROC                                     Uniform2fv;
        PFNGLUNIFORM2IPROC                                      Uniform2i;
        PFNGLUNIFORM2IVPROC                                     Uniform2iv;
        PFNGLUNIFORM2UIPROC                                     Uniform2ui;
        PFNGLUNIFORM2UIVPROC                                    Uniform2uiv;
        PFNGLUNIFORM3DPROC                                      Uniform3d;
        PFNGLUNIFORM3DVPROC                                     Uniform3dv;
        PFNGLUNIFORM3FPROC                                      Uniform3f;
        PFNGLUNIFORM3FVPROC                                     Uniform3fv;
        PFNGLUNIFORM3IPROC                                      Uniform3i;
        PFNGLUNIFORM3IVPROC                                     Uniform3iv;
        PFNGLUNIFORM3UIPROC                                     Uniform3ui;
        PFNGLUNIFORM3UIVPROC                                    Uniform3uiv;
        PFNGLUNIFORM4DPROC                                      Uniform4d;
        PFNGLUNIFORM4DVPROC                                     Uniform4dv;
        PFNGLUNIFORM4FPROC                                      Uniform4f;
        PFNGLUNIFORM4FVPROC                                     Uniform4fv;
        PFNGLUNIFORM4IPROC                                      Uniform4i;
        PFNGLUNIFORM4IVPROC                                     Uniform4iv;
        PFNGLUNIFORM4UIPROC                                     Uniform4ui;
        PFNGLUNIFORM4UIVPROC                                    Uniform4uiv;
        PFNGLUNIFORMBLOCKBINDINGPROC                            UniformBlockBinding;
        PFNGLUNIFORMMATRIX2DVPROC                               UniformMatrix2dv;
        PFNGLUNIFORMMATRIX2FVPROC                               UniformMatrix2fv;
        PFNGLUNIFORMMATRIX2X3DVPROC                             UniformMatrix2x3dv;
        PFNGLUNIFORMMATRIX2X3FVPROC                             UniformMatrix2x3fv;
        PFNGLUNIFORMMATRIX2X4DVPROC                             UniformMatrix2x4dv;
        PFNGLUNIFORMMATRIX2X4FVPROC                             UniformMatrix2x4fv;
        PFNGLUNIFORMMATRIX3DVPROC                               UniformMatrix3dv;
        PFNGLUNIFORMMATRIX3FVPROC                               UniformMatrix3fv;
        PFNGLUNIFORMMATRIX3X2DVPROC                             UniformMatrix3x2dv;
        PFNGLUNIFORMMATRIX3X2FVPROC                             UniformMatrix3x2fv;
        PFNGLUNIFORMMATRIX3X4DVPROC                             UniformMatrix3x4dv;
        PFNGLUNIFORMMATRIX3X4FVPROC                             UniformMatrix3x4fv;
        PFNGLUNIFORMMATRIX4DVPROC                               UniformMatrix4dv;
        PFNGLUNIFORMMATRIX4FVPROC                               UniformMatrix4fv;
        PFNGLUNIFORMMATRIX4X2DVPROC                             UniformMatrix4x2dv;
        PFNGLUNIFORMMATRIX4X2FVPROC                             UniformMatrix4x2fv;
        PFNGLUNIFORMMATRIX4X3DVPROC                             UniformMatrix4x3dv;
        PFNGLUNIFORMMATRIX4X3FVPROC                             UniformMatrix4x3fv;
        PFNGLUNIFORMSUBROUTINESUIVPROC                          UniformSubroutinesuiv;
        PFNGLUNMAPBUFFERPROC                                    UnmapBuffer;
        PFNGLUNMAPNAMEDBUFFERPROC                               UnmapNamedBuffer;
        PFNGLUSEPROGRAMPROC                                     UseProgram;
        PFNGLUSEPROGRAMSTAGESPROC                               UseProgramStages;
        PFNGLVALIDATEPROGRAMPROC                                ValidateProgram;
        PFNGLVALIDATEPROGRAMPIPELINEPROC                        ValidateProgramPipeline;
        PFNGLVERTEXARRAYATTRIBBINDINGPROC                       VertexArrayAttribBinding;
        PFNGLVERTEXARRAYATTRIBFORMATPROC                        VertexArrayAttribFormat;
        PFNGLVERTEXARRAYATTRIBIFORMATPROC                       VertexArrayAttribIFormat;
        PFNGLVERTEXARRAYATTRIBLFORMATPROC                       VertexArrayAttribLFormat;
        PFNGLVERTEXARRAYBINDINGDIVISORPROC                      VertexArrayBindingDivisor;
        PFNGLVERTEXARRAYELEMENTBUFFERPROC                       VertexArrayElementBuffer;
        PFNGLVERTEXARRAYVERTEXBUFFERPROC                        VertexArrayVertexBuffer;
        PFNGLVERTEXARRAYVERTEXBUFFERSPROC                       VertexArrayVertexBuffers;
        PFNGLVERTEXATTRIB1DPROC                                 VertexAttrib1d;
        PFNGLVERTEXATTRIB1DVPROC                                VertexAttrib1dv;
        PFNGLVERTEXATTRIB1FPROC                                 VertexAttrib1f;
        PFNGLVERTEXATTRIB1FVPROC                                VertexAttrib1fv;
        PFNGLVERTEXATTRIB1SPROC                                 VertexAttrib1s;
        PFNGLVERTEXATTRIB1SVPROC                                VertexAttrib1sv;
        PFNGLVERTEXATTRIB2DPROC                                 VertexAttrib2d;
        PFNGLVERTEXATTRIB2DVPROC                                VertexAttrib2dv;
        PFNGLVERTEXATTRIB2FPROC                                 VertexAttrib2f;
        PFNGLVERTEXATTRIB2FVPROC                                VertexAttrib2fv;
        PFNGLVERTEXATTRIB2SPROC                                 VertexAttrib2s;
        PFNGLVERTEXATTRIB2SVPROC                                VertexAttrib2sv;
        PFNGLVERTEXATTRIB3DPROC                                 VertexAttrib3d;
        PFNGLVERTEXATTRIB3DVPROC                                VertexAttrib3dv;
        PFNGLVERTEXATTRIB3FPROC                                 VertexAttrib3f;
        PFNGLVERTEXATTRIB3FVPROC                                VertexAttrib3fv;
        PFNGLVERTEXATTRIB3SPROC                                 VertexAttrib3s;
        PFNGLVERTEXATTRIB3SVPROC                                VertexAttrib3sv;
        PFNGLVERTEXATTRIB4NBVPROC                               VertexAttrib4Nbv;
        PFNGLVERTEXATTRIB4NIVPROC                               VertexAttrib4Niv;
        PFNGLVERTEXATTRIB4NSVPROC                               VertexAttrib4Nsv;
        PFNGLVERTEXATTRIB4NUBPROC                               VertexAttrib4Nub;
        PFNGLVERTEXATTRIB4NUBVPROC                              VertexAttrib4Nubv;
        PFNGLVERTEXATTRIB4NUIVPROC                              VertexAttrib4Nuiv;
        PFNGLVERTEXATTRIB4NUSVPROC                              VertexAttrib4Nusv;
        PFNGLVERTEXATTRIB4BVPROC                                VertexAttrib4bv;
        PFNGLVERTEXATTRIB4DPROC                                 VertexAttrib4d;
        PFNGLVERTEXATTRIB4DVPROC                                VertexAttrib4dv;
        PFNGLVERTEXATTRIB4FPROC                                 VertexAttrib4f;
        PFNGLVERTEXATTRIB4FVPROC                                VertexAttrib4fv;
        PFNGLVERTEXATTRIB4IVPROC                                VertexAttrib4iv;
        PFNGLVERTEXATTRIB4SPROC                                 VertexAttrib4s;
        PFNGLVERTEXATTRIB4SVPROC                                VertexAttrib4sv;
        PFNGLVERTEXATTRIB4UBVPROC                               VertexAttrib4ubv;
        PFNGLVERTEXATTRIB4UIVPROC                               VertexAttrib4uiv;
        PFNGLVERTEXATTRIB4USVPROC                               VertexAttrib4usv;
        PFNGLVERTEXATTRIBBINDINGPROC                            VertexAttribBinding;
        PFNGLVERTEXATTRIBDIVISORPROC                            VertexAttribDivisor;
        PFNGLVERTEXATTRIBFORMATPROC                             VertexAttribFormat;
        PFNGLVERTEXATTRIBI1IPROC                                VertexAttribI1i;
        PFNGLVERTEXATTRIBI1IVPROC                               VertexAttribI1iv;
        PFNGLVERTEXATTRIBI1UIPROC                               VertexAttribI1ui;
        PFNGLVERTEXATTRIBI1UIVPROC                              VertexAttribI1uiv;
        PFNGLVERTEXATTRIBI2IPROC                                VertexAttribI2i;
        PFNGLVERTEXATTRIBI2IVPROC                               VertexAttribI2iv;
        PFNGLVERTEXATTRIBI2UIPROC                               VertexAttribI2ui;
        PFNGLVERTEXATTRIBI2UIVPROC                              VertexAttribI2uiv;
        PFNGLVERTEXATTRIBI3IPROC                                VertexAttribI3i;
        PFNGLVERTEXATTRIBI3IVPROC                               VertexAttribI3iv;
        PFNGLVERTEXATTRIBI3UIPROC                               VertexAttribI3ui;
        PFNGLVERTEXATTRIBI3UIVPROC                              VertexAttribI3uiv;
        PFNGLVERTEXATTRIBI4BVPROC                               VertexAttribI4bv;
        PFNGLVERTEXATTRIBI4IPROC                                VertexAttribI4i;
        PFNGLVERTEXATTRIBI4IVPROC                               VertexAttribI4iv;
        PFNGLVERTEXATTRIBI4SVPROC                               VertexAttribI4sv;
        PFNGLVERTEXATTRIBI4UBVPROC                              VertexAttribI4ubv;
        PFNGLVERTEXATTRIBI4UIPROC                               VertexAttribI4ui;
        PFNGLVERTEXATTRIBI4UIVPROC                              VertexAttribI4uiv;
        PFNGLVERTEXATTRIBI4USVPROC                              VertexAttribI4usv;
        PFNGLVERTEXATTRIBIFORMATPROC                            VertexAttribIFormat;
        PFNGLVERTEXATTRIBIPOINTERPROC                           VertexAttribIPointer;
        PFNGLVERTEXATTRIBL1DPROC                                VertexAttribL1d;
        PFNGLVERTEXATTRIBL1DVPROC                               VertexAttribL1dv;
        PFNGLVERTEXATTRIBL2DPROC                                VertexAttribL2d;
        PFNGLVERTEXATTRIBL2DVPROC                               VertexAttribL2dv;
        PFNGLVERTEXATTRIBL3DPROC                                VertexAttribL3d;
        PFNGLVERTEXATTRIBL3DVPROC                               VertexAttribL3dv;
        PFNGLVERTEXATTRIBL4DPROC                                VertexAttribL4d;
        PFNGLVERTEXATTRIBL4DVPROC                               VertexAttribL4dv;
        PFNGLVERTEXATTRIBLFORMATPROC                            VertexAttribLFormat;
        PFNGLVERTEXATTRIBLPOINTERPROC                           VertexAttribLPointer;
        PFNGLVERTEXATTRIBP1UIPROC                               VertexAttribP1ui;
        PFNGLVERTEXATTRIBP1UIVPROC                              VertexAttribP1uiv;
        PFNGLVERTEXATTRIBP2UIPROC                               VertexAttribP2ui;
        PFNGLVERTEXATTRIBP2UIVPROC                              VertexAttribP2uiv;
        PFNGLVERTEXATTRIBP3UIPROC                               VertexAttribP3ui;
        PFNGLVERTEXATTRIBP3UIVPROC                              VertexAttribP3uiv;
        PFNGLVERTEXATTRIBP4UIPROC                               VertexAttribP4ui;
        PFNGLVERTEXATTRIBP4UIVPROC                              VertexAttribP4uiv;
        PFNGLVERTEXATTRIBPOINTERPROC                            VertexAttribPointer;
        PFNGLVERTEXBINDINGDIVISORPROC                           VertexBindingDivisor;
        PFNGLVIEWPORTPROC                                       Viewport;
        PFNGLVIEWPORTARRAYVPROC                                 ViewportArrayv;
        PFNGLVIEWPORTINDEXEDFPROC                               ViewportIndexedf;
        PFNGLVIEWPORTINDEXEDFVPROC                              ViewportIndexedfv;
        PFNGLWAITSYNCPROC                                       WaitSync;
    } gl;
};

extern union VGLProcs *vglProcs;

/* OpenGL functions */
#define vglActiveShaderProgram                            vglProcs->gl.ActiveShaderProgram
#define vglActiveTexture                                  vglProcs->gl.ActiveTexture
#define vglAttachShader                                   vglProcs->gl.AttachShader
#define vglBeginConditionalRender                         vglProcs->gl.BeginConditionalRender
#define vglBeginQuery                                     vglProcs->gl.BeginQuery
#define vglBeginQueryIndexed                              vglProcs->gl.BeginQueryIndexed
#define vglBeginTransformFeedback                         vglProcs->gl.BeginTransformFeedback
#define vglBindAttribLocation                             vglProcs->gl.BindAttribLocation
#define vglBindBuffer                                     vglProcs->gl.BindBuffer
#define vglBindBufferBase                                 vglProcs->gl.BindBufferBase
#define vglBindBufferRange                                vglProcs->gl.BindBufferRange
#define vglBindBuffersBase                                vglProcs->gl.BindBuffersBase
#define vglBindBuffersRange                               vglProcs->gl.BindBuffersRange
#define vglBindFragDataLocation                           vglProcs->gl.BindFragDataLocation
#define vglBindFragDataLocationIndexed                    vglProcs->gl.BindFragDataLocationIndexed
#define vglBindFramebuffer                                vglProcs->gl.BindFramebuffer
#define vglBindImageTexture                               vglProcs->gl.BindImageTexture
#define vglBindImageTextures                              vglProcs->gl.BindImageTextures
#define vglBindProgramPipeline                            vglProcs->gl.BindProgramPipeline
#define vglBindRenderbuffer                               vglProcs->gl.BindRenderbuffer
#define vglBindSampler                                    vglProcs->gl.BindSampler
#define vglBindSamplers                                   vglProcs->gl.BindSamplers
#define vglBindTexture                                    vglProcs->gl.BindTexture
#define vglBindTextureUnit                                vglProcs->gl.BindTextureUnit
#define vglBindTextures                                   vglProcs->gl.BindTextures
#define vglBindTransformFeedback                          vglProcs->gl.BindTransformFeedback
#define vglBindVertexArray                                vglProcs->gl.BindVertexArray
#define vglBindVertexBuffer                               vglProcs->gl.BindVertexBuffer
#define vglBindVertexBuffers                              vglProcs->gl.BindVertexBuffers
#define vglBlendColor                                     vglProcs->gl.BlendColor
#define vglBlendEquation                                  vglProcs->gl.BlendEquation
#define vglBlendEquationSeparate                          vglProcs->gl.BlendEquationSeparate
#define vglBlendEquationSeparatei                         vglProcs->gl.BlendEquationSeparatei
#define vglBlendEquationi                                 vglProcs->gl.BlendEquationi
#define vglBlendFunc                                      vglProcs->gl.BlendFunc
#define vglBlendFuncSeparate                              vglProcs->gl.BlendFuncSeparate
#define vglBlendFuncSeparatei                             vglProcs->gl.BlendFuncSeparatei
#define vglBlendFunci                                     vglProcs->gl.BlendFunci
#define vglBlitFramebuffer                                vglProcs->gl.BlitFramebuffer
#define vglBlitNamedFramebuffer                           vglProcs->gl.BlitNamedFramebuffer
#define vglBufferData                                     vglProcs->gl.BufferData
#define vglBufferStorage                                  vglProcs->gl.BufferStorage
#define vglBufferSubData                                  vglProcs->gl.BufferSubData
#define vglCheckFramebufferStatus                         vglProcs->gl.CheckFramebufferStatus
#define vglCheckNamedFramebufferStatus                    vglProcs->gl.CheckNamedFramebufferStatus
#define vglClampColor                                     vglProcs->gl.ClampColor
#define vglClear                                          vglProcs->gl.Clear
#define vglClearBufferData                                vglProcs->gl.ClearBufferData
#define vglClearBufferSubData                             vglProcs->gl.ClearBufferSubData
#define vglClearBufferfi                                  vglProcs->gl.ClearBufferfi
#define vglClearBufferfv                                  vglProcs->gl.ClearBufferfv
#define vglClearBufferiv                                  vglProcs->gl.ClearBufferiv
#define vglClearBufferuiv                                 vglProcs->gl.ClearBufferuiv
#define vglClearColor                                     vglProcs->gl.ClearColor
#define vglClearDepth                                     vglProcs->gl.ClearDepth
#define vglClearDepthf                                    vglProcs->gl.ClearDepthf
#define vglClearNamedBufferData                           vglProcs->gl.ClearNamedBufferData
#define vglClearNamedBufferSubData                        vglProcs->gl.ClearNamedBufferSubData
#define vglClearNamedFramebufferfi                        vglProcs->gl.ClearNamedFramebufferfi
#define vglClearNamedFramebufferfv                        vglProcs->gl.ClearNamedFramebufferfv
#define vglClearNamedFramebufferiv                        vglProcs->gl.ClearNamedFramebufferiv
#define vglClearNamedFramebufferuiv                       vglProcs->gl.ClearNamedFramebufferuiv
#define vglClearStencil                                   vglProcs->gl.ClearStencil
#define vglClearTexImage                                  vglProcs->gl.ClearTexImage
#define vglClearTexSubImage                               vglProcs->gl.ClearTexSubImage
#define vglClientWaitSync                                 vglProcs->gl.ClientWaitSync
#define vglClipControl                                    vglProcs->gl.ClipControl
#define vglColorMask                                      vglProcs->gl.ColorMask
#define vglColorMaski                                     vglProcs->gl.ColorMaski
#define vglCompileShader                                  vglProcs->gl.CompileShader
#define vglCompressedTexImage1D                           vglProcs->gl.CompressedTexImage1D
#define vglCompressedTexImage2D                           vglProcs->gl.CompressedTexImage2D
#define vglCompressedTexImage3D                           vglProcs->gl.CompressedTexImage3D
#define vglCompressedTexSubImage1D                        vglProcs->gl.CompressedTexSubImage1D
#define vglCompressedTexSubImage2D                        vglProcs->gl.CompressedTexSubImage2D
#define vglCompressedTexSubImage3D                        vglProcs->gl.CompressedTexSubImage3D
#define vglCompressedTextureSubImage1D                    vglProcs->gl.CompressedTextureSubImage1D
#define vglCompressedTextureSubImage2D                    vglProcs->gl.CompressedTextureSubImage2D
#define vglCompressedTextureSubImage3D                    vglProcs->gl.CompressedTextureSubImage3D
#define vglCopyBufferSubData                              vglProcs->gl.CopyBufferSubData
#define vglCopyImageSubData                               vglProcs->gl.CopyImageSubData
#define vglCopyNamedBufferSubData                         vglProcs->gl.CopyNamedBufferSubData
#define vglCopyTexImage1D                                 vglProcs->gl.CopyTexImage1D
#define vglCopyTexImage2D                                 vglProcs->gl.CopyTexImage2D
#define vglCopyTexSubImage1D                              vglProcs->gl.CopyTexSubImage1D
#define vglCopyTexSubImage2D                              vglProcs->gl.CopyTexSubImage2D
#define vglCopyTexSubImage3D                              vglProcs->gl.CopyTexSubImage3D
#define vglCopyTextureSubImage1D                          vglProcs->gl.CopyTextureSubImage1D
#define vglCopyTextureSubImage2D                          vglProcs->gl.CopyTextureSubImage2D
#define vglCopyTextureSubImage3D                          vglProcs->gl.CopyTextureSubImage3D
#define vglCreateBuffers                                  vglProcs->gl.CreateBuffers
#define vglCreateFramebuffers                             vglProcs->gl.CreateFramebuffers
#define vglCreateProgram                                  vglProcs->gl.CreateProgram
#define vglCreateProgramPipelines                         vglProcs->gl.CreateProgramPipelines
#define vglCreateQueries                                  vglProcs->gl.CreateQueries
#define vglCreateRenderbuffers                            vglProcs->gl.CreateRenderbuffers
#define vglCreateSamplers                                 vglProcs->gl.CreateSamplers
#define vglCreateShader                                   vglProcs->gl.CreateShader
#define vglCreateShaderProgramv                           vglProcs->gl.CreateShaderProgramv
#define vglCreateTextures                                 vglProcs->gl.CreateTextures
#define vglCreateTransformFeedbacks                       vglProcs->gl.CreateTransformFeedbacks
#define vglCreateVertexArrays                             vglProcs->gl.CreateVertexArrays
#define vglCullFace                                       vglProcs->gl.CullFace
#define vglDebugMessageCallback                           vglProcs->gl.DebugMessageCallback
#define vglDebugMessageControl                            vglProcs->gl.DebugMessageControl
#define vglDebugMessageInsert                             vglProcs->gl.DebugMessageInsert
#define vglDeleteBuffers                                  vglProcs->gl.DeleteBuffers
#define vglDeleteFramebuffers                             vglProcs->gl.DeleteFramebuffers
#define vglDeleteProgram                                  vglProcs->gl.DeleteProgram
#define vglDeleteProgramPipelines                         vglProcs->gl.DeleteProgramPipelines
#define vglDeleteQueries                                  vglProcs->gl.DeleteQueries
#define vglDeleteRenderbuffers                            vglProcs->gl.DeleteRenderbuffers
#define vglDeleteSamplers                                 vglProcs->gl.DeleteSamplers
#define vglDeleteShader                                   vglProcs->gl.DeleteShader
#define vglDeleteSync                                     vglProcs->gl.DeleteSync
#define vglDeleteTextures                                 vglProcs->gl.DeleteTextures
#define vglDeleteTransformFeedbacks                       vglProcs->gl.DeleteTransformFeedbacks
#define vglDeleteVertexArrays                             vglProcs->gl.DeleteVertexArrays
#define vglDepthFunc                                      vglProcs->gl.DepthFunc
#define vglDepthMask                                      vglProcs->gl.DepthMask
#define vglDepthRange                                     vglProcs->gl.DepthRange
#define vglDepthRangeArrayv                               vglProcs->gl.DepthRangeArrayv
#define vglDepthRangeIndexed                              vglProcs->gl.DepthRangeIndexed
#define vglDepthRangef                                    vglProcs->gl.DepthRangef
#define vglDetachShader                                   vglProcs->gl.DetachShader
#define vglDisable                                        vglProcs->gl.Disable
#define vglDisableVertexArrayAttrib                       vglProcs->gl.DisableVertexArrayAttrib
#define vglDisableVertexAttribArray                       vglProcs->gl.DisableVertexAttribArray
#define vglDisablei                                       vglProcs->gl.Disablei
#define vglDispatchCompute                                vglProcs->gl.DispatchCompute
#define vglDispatchComputeIndirect                        vglProcs->gl.DispatchComputeIndirect
#define vglDrawArrays                                     vglProcs->gl.DrawArrays
#define vglDrawArraysIndirect                             vglProcs->gl.DrawArraysIndirect
#define vglDrawArraysInstanced                            vglProcs->gl.DrawArraysInstanced
#define vglDrawArraysInstancedBaseInstance                vglProcs->gl.DrawArraysInstancedBaseInstance
#define vglDrawBuffer                                     vglProcs->gl.DrawBuffer
#define vglDrawBuffers                                    vglProcs->gl.DrawBuffers
#define vglDrawElements                                   vglProcs->gl.DrawElements
#define vglDrawElementsBaseVertex                         vglProcs->gl.DrawElementsBaseVertex
#define vglDrawElementsIndirect                           vglProcs->gl.DrawElementsIndirect
#define vglDrawElementsInstanced                          vglProcs->gl.DrawElementsInstanced
#define vglDrawElementsInstancedBaseInstance              vglProcs->gl.DrawElementsInstancedBaseInstance
#define vglDrawElementsInstancedBaseVertex                vglProcs->gl.DrawElementsInstancedBaseVertex
#define vglDrawElementsInstancedBaseVertexBaseInstance    vglProcs->gl.DrawElementsInstancedBaseVertexBaseInstance
#define vglDrawRangeElements                              vglProcs->gl.DrawRangeElements
#define vglDrawRangeElementsBaseVertex                    vglProcs->gl.DrawRangeElementsBaseVertex
#define vglDrawTransformFeedback                          vglProcs->gl.DrawTransformFeedback
#define vglDrawTransformFeedbackInstanced                 vglProcs->gl.DrawTransformFeedbackInstanced
#define vglDrawTransformFeedbackStream                    vglProcs->gl.DrawTransformFeedbackStream
#define vglDrawTransformFeedbackStreamInstanced           vglProcs->gl.DrawTransformFeedbackStreamInstanced
#define vglEnable                                         vglProcs->gl.Enable
#define vglEnableVertexArrayAttrib                        vglProcs->gl.EnableVertexArrayAttrib
#define vglEnableVertexAttribArray                        vglProcs->gl.EnableVertexAttribArray
#define vglEnablei                                        vglProcs->gl.Enablei
#define vglEndConditionalRender                           vglProcs->gl.EndConditionalRender
#define vglEndQuery                                       vglProcs->gl.EndQuery
#define vglEndQueryIndexed                                vglProcs->gl.EndQueryIndexed
#define vglEndTransformFeedback                           vglProcs->gl.EndTransformFeedback
#define vglFenceSync                                      vglProcs->gl.FenceSync
#define vglFinish                                         vglProcs->gl.Finish
#define vglFlush                                          vglProcs->gl.Flush
#define vglFlushMappedBufferRange                         vglProcs->gl.FlushMappedBufferRange
#define vglFlushMappedNamedBufferRange                    vglProcs->gl.FlushMappedNamedBufferRange
#define vglFramebufferParameteri                          vglProcs->gl.FramebufferParameteri
#define vglFramebufferParameteriMESA                      vglProcs->gl.FramebufferParameteriMESA
#define vglFramebufferRenderbuffer                        vglProcs->gl.FramebufferRenderbuffer
#define vglFramebufferTexture                             vglProcs->gl.FramebufferTexture
#define vglFramebufferTexture1D                           vglProcs->gl.FramebufferTexture1D
#define vglFramebufferTexture2D                           vglProcs->gl.FramebufferTexture2D
#define vglFramebufferTexture3D                           vglProcs->gl.FramebufferTexture3D
#define vglFramebufferTextureLayer                        vglProcs->gl.FramebufferTextureLayer
#define vglFrontFace                                      vglProcs->gl.FrontFace
#define vglGenBuffers                                     vglProcs->gl.GenBuffers
#define vglGenFramebuffers                                vglProcs->gl.GenFramebuffers
#define vglGenProgramPipelines                            vglProcs->gl.GenProgramPipelines
#define vglGenQueries                                     vglProcs->gl.GenQueries
#define vglGenRenderbuffers                               vglProcs->gl.GenRenderbuffers
#define vglGenSamplers                                    vglProcs->gl.GenSamplers
#define vglGenTextures                                    vglProcs->gl.GenTextures
#define vglGenTransformFeedbacks                          vglProcs->gl.GenTransformFeedbacks
#define vglGenVertexArrays                                vglProcs->gl.GenVertexArrays
#define vglGenerateMipmap                                 vglProcs->gl.GenerateMipmap
#define vglGenerateTextureMipmap                          vglProcs->gl.GenerateTextureMipmap
#define vglGetActiveAtomicCounterBufferiv                 vglProcs->gl.GetActiveAtomicCounterBufferiv
#define vglGetActiveAttrib                                vglProcs->gl.GetActiveAttrib
#define vglGetActiveSubroutineName                        vglProcs->gl.GetActiveSubroutineName
#define vglGetActiveSubroutineUniformName                 vglProcs->gl.GetActiveSubroutineUniformName
#define vglGetActiveSubroutineUniformiv                   vglProcs->gl.GetActiveSubroutineUniformiv
#define vglGetActiveUniform                               vglProcs->gl.GetActiveUniform
#define vglGetActiveUniformBlockName                      vglProcs->gl.GetActiveUniformBlockName
#define vglGetActiveUniformBlockiv                        vglProcs->gl.GetActiveUniformBlockiv
#define vglGetActiveUniformName                           vglProcs->gl.GetActiveUniformName
#define vglGetActiveUniformsiv                            vglProcs->gl.GetActiveUniformsiv
#define vglGetAttachedShaders                             vglProcs->gl.GetAttachedShaders
#define vglGetAttribLocation                              vglProcs->gl.GetAttribLocation
#define vglGetBooleani_v                                  vglProcs->gl.GetBooleani_v
#define vglGetBooleanv                                    vglProcs->gl.GetBooleanv
#define vglGetBufferParameteri64v                         vglProcs->gl.GetBufferParameteri64v
#define vglGetBufferParameteriv                           vglProcs->gl.GetBufferParameteriv
#define vglGetBufferPointerv                              vglProcs->gl.GetBufferPointerv
#define vglGetBufferSubData                               vglProcs->gl.GetBufferSubData
#define vglGetCompressedTexImage                          vglProcs->gl.GetCompressedTexImage
#define vglGetCompressedTextureImage                      vglProcs->gl.GetCompressedTextureImage
#define vglGetCompressedTextureSubImage                   vglProcs->gl.GetCompressedTextureSubImage
#define vglGetDebugMessageLog                             vglProcs->gl.GetDebugMessageLog
#define vglGetDoublei_v                                   vglProcs->gl.GetDoublei_v
#define vglGetDoublev                                     vglProcs->gl.GetDoublev
#define vglGetError                                       vglProcs->gl.GetError
#define vglGetFloati_v                                    vglProcs->gl.GetFloati_v
#define vglGetFloatv                                      vglProcs->gl.GetFloatv
#define vglGetFragDataIndex                               vglProcs->gl.GetFragDataIndex
#define vglGetFragDataLocation                            vglProcs->gl.GetFragDataLocation
#define vglGetFramebufferAttachmentParameteriv            vglProcs->gl.GetFramebufferAttachmentParameteriv
#define vglGetFramebufferParameteriv                      vglProcs->gl.GetFramebufferParameteriv
#define vglGetFramebufferParameterivMESA                  vglProcs->gl.GetFramebufferParameterivMESA
#define vglGetGraphicsResetStatus                         vglProcs->gl.GetGraphicsResetStatus
#define vglGetInteger64i_v                                vglProcs->gl.GetInteger64i_v
#define vglGetInteger64v                                  vglProcs->gl.GetInteger64v
#define vglGetIntegeri_v                                  vglProcs->gl.GetIntegeri_v
#define vglGetIntegerv                                    vglProcs->gl.GetIntegerv
#define vglGetInternalformati64v                          vglProcs->gl.GetInternalformati64v
#define vglGetInternalformativ                            vglProcs->gl.GetInternalformativ
#define vglGetMultisamplefv                               vglProcs->gl.GetMultisamplefv
#define vglGetNamedBufferParameteri64v                    vglProcs->gl.GetNamedBufferParameteri64v
#define vglGetNamedBufferParameteriv                      vglProcs->gl.GetNamedBufferParameteriv
#define vglGetNamedBufferPointerv                         vglProcs->gl.GetNamedBufferPointerv
#define vglGetNamedBufferSubData                          vglProcs->gl.GetNamedBufferSubData
#define vglGetNamedFramebufferAttachmentParameteriv       vglProcs->gl.GetNamedFramebufferAttachmentParameteriv
#define vglGetNamedFramebufferParameteriv                 vglProcs->gl.GetNamedFramebufferParameteriv
#define vglGetNamedRenderbufferParameteriv                vglProcs->gl.GetNamedRenderbufferParameteriv
#define vglGetObjectLabel                                 vglProcs->gl.GetObjectLabel
#define vglGetObjectPtrLabel                              vglProcs->gl.GetObjectPtrLabel
#define vglGetPointerv                                    vglProcs->gl.GetPointerv
#define vglGetProgramBinary                               vglProcs->gl.GetProgramBinary
#define vglGetProgramInfoLog                              vglProcs->gl.GetProgramInfoLog
#define vglGetProgramInterfaceiv                          vglProcs->gl.GetProgramInterfaceiv
#define vglGetProgramPipelineInfoLog                      vglProcs->gl.GetProgramPipelineInfoLog
#define vglGetProgramPipelineiv                           vglProcs->gl.GetProgramPipelineiv
#define vglGetProgramResourceIndex                        vglProcs->gl.GetProgramResourceIndex
#define vglGetProgramResourceLocation                     vglProcs->gl.GetProgramResourceLocation
#define vglGetProgramResourceLocationIndex                vglProcs->gl.GetProgramResourceLocationIndex
#define vglGetProgramResourceName                         vglProcs->gl.GetProgramResourceName
#define vglGetProgramResourceiv                           vglProcs->gl.GetProgramResourceiv
#define vglGetProgramStageiv                              vglProcs->gl.GetProgramStageiv
#define vglGetProgramiv                                   vglProcs->gl.GetProgramiv
#define vglGetQueryBufferObjecti64v                       vglProcs->gl.GetQueryBufferObjecti64v
#define vglGetQueryBufferObjectiv                         vglProcs->gl.GetQueryBufferObjectiv
#define vglGetQueryBufferObjectui64v                      vglProcs->gl.GetQueryBufferObjectui64v
#define vglGetQueryBufferObjectuiv                        vglProcs->gl.GetQueryBufferObjectuiv
#define vglGetQueryIndexediv                              vglProcs->gl.GetQueryIndexediv
#define vglGetQueryObjecti64v                             vglProcs->gl.GetQueryObjecti64v
#define vglGetQueryObjectiv                               vglProcs->gl.GetQueryObjectiv
#define vglGetQueryObjectui64v                            vglProcs->gl.GetQueryObjectui64v
#define vglGetQueryObjectuiv                              vglProcs->gl.GetQueryObjectuiv
#define vglGetQueryiv                                     vglProcs->gl.GetQueryiv
#define vglGetRenderbufferParameteriv                     vglProcs->gl.GetRenderbufferParameteriv
#define vglGetSamplerParameterIiv                         vglProcs->gl.GetSamplerParameterIiv
#define vglGetSamplerParameterIuiv                        vglProcs->gl.GetSamplerParameterIuiv
#define vglGetSamplerParameterfv                          vglProcs->gl.GetSamplerParameterfv
#define vglGetSamplerParameteriv                          vglProcs->gl.GetSamplerParameteriv
#define vglGetShaderInfoLog                               vglProcs->gl.GetShaderInfoLog
#define vglGetShaderPrecisionFormat                       vglProcs->gl.GetShaderPrecisionFormat
#define vglGetShaderSource                                vglProcs->gl.GetShaderSource
#define vglGetShaderiv                                    vglProcs->gl.GetShaderiv
#define vglGetString                                      vglProcs->gl.GetString
#define vglGetStringi                                     vglProcs->gl.GetStringi
#define vglGetSubroutineIndex                             vglProcs->gl.GetSubroutineIndex
#define vglGetSubroutineUniformLocation                   vglProcs->gl.GetSubroutineUniformLocation
#define vglGetSynciv                                      vglProcs->gl.GetSynciv
#define vglGetTexImage                                    vglProcs->gl.GetTexImage
#define vglGetTexLevelParameterfv                         vglProcs->gl.GetTexLevelParameterfv
#define vglGetTexLevelParameteriv                         vglProcs->gl.GetTexLevelParameteriv
#define vglGetTexParameterIiv                             vglProcs->gl.GetTexParameterIiv
#define vglGetTexParameterIuiv                            vglProcs->gl.GetTexParameterIuiv
#define vglGetTexParameterfv                              vglProcs->gl.GetTexParameterfv
#define vglGetTexParameteriv                              vglProcs->gl.GetTexParameteriv
#define vglGetTextureImage                                vglProcs->gl.GetTextureImage
#define vglGetTextureLevelParameterfv                     vglProcs->gl.GetTextureLevelParameterfv
#define vglGetTextureLevelParameteriv                     vglProcs->gl.GetTextureLevelParameteriv
#define vglGetTextureParameterIiv                         vglProcs->gl.GetTextureParameterIiv
#define vglGetTextureParameterIuiv                        vglProcs->gl.GetTextureParameterIuiv
#define vglGetTextureParameterfv                          vglProcs->gl.GetTextureParameterfv
#define vglGetTextureParameteriv                          vglProcs->gl.GetTextureParameteriv
#define vglGetTextureSubImage                             vglProcs->gl.GetTextureSubImage
#define vglGetTransformFeedbackVarying                    vglProcs->gl.GetTransformFeedbackVarying
#define vglGetTransformFeedbacki64_v                      vglProcs->gl.GetTransformFeedbacki64_v
#define vglGetTransformFeedbacki_v                        vglProcs->gl.GetTransformFeedbacki_v
#define vglGetTransformFeedbackiv                         vglProcs->gl.GetTransformFeedbackiv
#define vglGetUniformBlockIndex                           vglProcs->gl.GetUniformBlockIndex
#define vglGetUniformIndices                              vglProcs->gl.GetUniformIndices
#define vglGetUniformLocation                             vglProcs->gl.GetUniformLocation
#define vglGetUniformSubroutineuiv                        vglProcs->gl.GetUniformSubroutineuiv
#define vglGetUniformdv                                   vglProcs->gl.GetUniformdv
#define vglGetUniformfv                                   vglProcs->gl.GetUniformfv
#define vglGetUniformiv                                   vglProcs->gl.GetUniformiv
#define vglGetUniformuiv                                  vglProcs->gl.GetUniformuiv
#define vglGetVertexArrayIndexed64iv                      vglProcs->gl.GetVertexArrayIndexed64iv
#define vglGetVertexArrayIndexediv                        vglProcs->gl.GetVertexArrayIndexediv
#define vglGetVertexArrayiv                               vglProcs->gl.GetVertexArrayiv
#define vglGetVertexAttribIiv                             vglProcs->gl.GetVertexAttribIiv
#define vglGetVertexAttribIuiv                            vglProcs->gl.GetVertexAttribIuiv
#define vglGetVertexAttribLdv                             vglProcs->gl.GetVertexAttribLdv
#define vglGetVertexAttribPointerv                        vglProcs->gl.GetVertexAttribPointerv
#define vglGetVertexAttribdv                              vglProcs->gl.GetVertexAttribdv
#define vglGetVertexAttribfv                              vglProcs->gl.GetVertexAttribfv
#define vglGetVertexAttribiv                              vglProcs->gl.GetVertexAttribiv
#define vglGetnCompressedTexImage                         vglProcs->gl.GetnCompressedTexImage
#define vglGetnTexImage                                   vglProcs->gl.GetnTexImage
#define vglGetnUniformdv                                  vglProcs->gl.GetnUniformdv
#define vglGetnUniformfv                                  vglProcs->gl.GetnUniformfv
#define vglGetnUniformiv                                  vglProcs->gl.GetnUniformiv
#define vglGetnUniformuiv                                 vglProcs->gl.GetnUniformuiv
#define vglHint                                           vglProcs->gl.Hint
#define vglInvalidateBufferData                           vglProcs->gl.InvalidateBufferData
#define vglInvalidateBufferSubData                        vglProcs->gl.InvalidateBufferSubData
#define vglInvalidateFramebuffer                          vglProcs->gl.InvalidateFramebuffer
#define vglInvalidateNamedFramebufferData                 vglProcs->gl.InvalidateNamedFramebufferData
#define vglInvalidateNamedFramebufferSubData              vglProcs->gl.InvalidateNamedFramebufferSubData
#define vglInvalidateSubFramebuffer                       vglProcs->gl.InvalidateSubFramebuffer
#define vglInvalidateTexImage                             vglProcs->gl.InvalidateTexImage
#define vglInvalidateTexSubImage                          vglProcs->gl.InvalidateTexSubImage
#define vglIsBuffer                                       vglProcs->gl.IsBuffer
#define vglIsEnabled                                      vglProcs->gl.IsEnabled
#define vglIsEnabledi                                     vglProcs->gl.IsEnabledi
#define vglIsFramebuffer                                  vglProcs->gl.IsFramebuffer
#define vglIsProgram                                      vglProcs->gl.IsProgram
#define vglIsProgramPipeline                              vglProcs->gl.IsProgramPipeline
#define vglIsQuery                                        vglProcs->gl.IsQuery
#define vglIsRenderbuffer                                 vglProcs->gl.IsRenderbuffer
#define vglIsSampler                                      vglProcs->gl.IsSampler
#define vglIsShader                                       vglProcs->gl.IsShader
#define vglIsSync                                         vglProcs->gl.IsSync
#define vglIsTexture                                      vglProcs->gl.IsTexture
#define vglIsTransformFeedback                            vglProcs->gl.IsTransformFeedback
#define vglIsVertexArray                                  vglProcs->gl.IsVertexArray
#define vglLineWidth                                      vglProcs->gl.LineWidth
#define vglLinkProgram                                    vglProcs->gl.LinkProgram
#define vglLogicOp                                        vglProcs->gl.LogicOp
#define vglMapBuffer                                      vglProcs->gl.MapBuffer
#define vglMapBufferRange                                 vglProcs->gl.MapBufferRange
#define vglMapNamedBuffer                                 vglProcs->gl.MapNamedBuffer
#define vglMapNamedBufferRange                            vglProcs->gl.MapNamedBufferRange
#define vglMemoryBarrier                                  vglProcs->gl.MemoryBarrier
#define vglMemoryBarrierByRegion                          vglProcs->gl.MemoryBarrierByRegion
#define vglMinSampleShading                               vglProcs->gl.MinSampleShading
#define vglMultiDrawArrays                                vglProcs->gl.MultiDrawArrays
#define vglMultiDrawArraysIndirect                        vglProcs->gl.MultiDrawArraysIndirect
#define vglMultiDrawArraysIndirectCount                   vglProcs->gl.MultiDrawArraysIndirectCount
#define vglMultiDrawElements                              vglProcs->gl.MultiDrawElements
#define vglMultiDrawElementsBaseVertex                    vglProcs->gl.MultiDrawElementsBaseVertex
#define vglMultiDrawElementsIndirect                      vglProcs->gl.MultiDrawElementsIndirect
#define vglMultiDrawElementsIndirectCount                 vglProcs->gl.MultiDrawElementsIndirectCount
#define vglNamedBufferData                                vglProcs->gl.NamedBufferData
#define vglNamedBufferStorage                             vglProcs->gl.NamedBufferStorage
#define vglNamedBufferSubData                             vglProcs->gl.NamedBufferSubData
#define vglNamedFramebufferDrawBuffer                     vglProcs->gl.NamedFramebufferDrawBuffer
#define vglNamedFramebufferDrawBuffers                    vglProcs->gl.NamedFramebufferDrawBuffers
#define vglNamedFramebufferParameteri                     vglProcs->gl.NamedFramebufferParameteri
#define vglNamedFramebufferReadBuffer                     vglProcs->gl.NamedFramebufferReadBuffer
#define vglNamedFramebufferRenderbuffer                   vglProcs->gl.NamedFramebufferRenderbuffer
#define vglNamedFramebufferTexture                        vglProcs->gl.NamedFramebufferTexture
#define vglNamedFramebufferTextureLayer                   vglProcs->gl.NamedFramebufferTextureLayer
#define vglNamedRenderbufferStorage                       vglProcs->gl.NamedRenderbufferStorage
#define vglNamedRenderbufferStorageMultisample            vglProcs->gl.NamedRenderbufferStorageMultisample
#define vglObjectLabel                                    vglProcs->gl.ObjectLabel
#define vglObjectPtrLabel                                 vglProcs->gl.ObjectPtrLabel
#define vglPatchParameterfv                               vglProcs->gl.PatchParameterfv
#define vglPatchParameteri                                vglProcs->gl.PatchParameteri
#define vglPauseTransformFeedback                         vglProcs->gl.PauseTransformFeedback
#define vglPixelStoref                                    vglProcs->gl.PixelStoref
#define vglPixelStorei                                    vglProcs->gl.PixelStorei
#define vglPointParameterf                                vglProcs->gl.PointParameterf
#define vglPointParameterfv                               vglProcs->gl.PointParameterfv
#define vglPointParameteri                                vglProcs->gl.PointParameteri
#define vglPointParameteriv                               vglProcs->gl.PointParameteriv
#define vglPointSize                                      vglProcs->gl.PointSize
#define vglPolygonMode                                    vglProcs->gl.PolygonMode
#define vglPolygonOffset                                  vglProcs->gl.PolygonOffset
#define vglPolygonOffsetClamp                             vglProcs->gl.PolygonOffsetClamp
#define vglPopDebugGroup                                  vglProcs->gl.PopDebugGroup
#define vglPrimitiveRestartIndex                          vglProcs->gl.PrimitiveRestartIndex
#define vglProgramBinary                                  vglProcs->gl.ProgramBinary
#define vglProgramParameteri                              vglProcs->gl.ProgramParameteri
#define vglProgramUniform1d                               vglProcs->gl.ProgramUniform1d
#define vglProgramUniform1dv                              vglProcs->gl.ProgramUniform1dv
#define vglProgramUniform1f                               vglProcs->gl.ProgramUniform1f
#define vglProgramUniform1fv                              vglProcs->gl.ProgramUniform1fv
#define vglProgramUniform1i                               vglProcs->gl.ProgramUniform1i
#define vglProgramUniform1iv                              vglProcs->gl.ProgramUniform1iv
#define vglProgramUniform1ui                              vglProcs->gl.ProgramUniform1ui
#define vglProgramUniform1uiv                             vglProcs->gl.ProgramUniform1uiv
#define vglProgramUniform2d                               vglProcs->gl.ProgramUniform2d
#define vglProgramUniform2dv                              vglProcs->gl.ProgramUniform2dv
#define vglProgramUniform2f                               vglProcs->gl.ProgramUniform2f
#define vglProgramUniform2fv                              vglProcs->gl.ProgramUniform2fv
#define vglProgramUniform2i                               vglProcs->gl.ProgramUniform2i
#define vglProgramUniform2iv                              vglProcs->gl.ProgramUniform2iv
#define vglProgramUniform2ui                              vglProcs->gl.ProgramUniform2ui
#define vglProgramUniform2uiv                             vglProcs->gl.ProgramUniform2uiv
#define vglProgramUniform3d                               vglProcs->gl.ProgramUniform3d
#define vglProgramUniform3dv                              vglProcs->gl.ProgramUniform3dv
#define vglProgramUniform3f                               vglProcs->gl.ProgramUniform3f
#define vglProgramUniform3fv                              vglProcs->gl.ProgramUniform3fv
#define vglProgramUniform3i                               vglProcs->gl.ProgramUniform3i
#define vglProgramUniform3iv                              vglProcs->gl.ProgramUniform3iv
#define vglProgramUniform3ui                              vglProcs->gl.ProgramUniform3ui
#define vglProgramUniform3uiv                             vglProcs->gl.ProgramUniform3uiv
#define vglProgramUniform4d                               vglProcs->gl.ProgramUniform4d
#define vglProgramUniform4dv                              vglProcs->gl.ProgramUniform4dv
#define vglProgramUniform4f                               vglProcs->gl.ProgramUniform4f
#define vglProgramUniform4fv                              vglProcs->gl.ProgramUniform4fv
#define vglProgramUniform4i                               vglProcs->gl.ProgramUniform4i
#define vglProgramUniform4iv                              vglProcs->gl.ProgramUniform4iv
#define vglProgramUniform4ui                              vglProcs->gl.ProgramUniform4ui
#define vglProgramUniform4uiv                             vglProcs->gl.ProgramUniform4uiv
#define vglProgramUniformMatrix2dv                        vglProcs->gl.ProgramUniformMatrix2dv
#define vglProgramUniformMatrix2fv                        vglProcs->gl.ProgramUniformMatrix2fv
#define vglProgramUniformMatrix2x3dv                      vglProcs->gl.ProgramUniformMatrix2x3dv
#define vglProgramUniformMatrix2x3fv                      vglProcs->gl.ProgramUniformMatrix2x3fv
#define vglProgramUniformMatrix2x4dv                      vglProcs->gl.ProgramUniformMatrix2x4dv
#define vglProgramUniformMatrix2x4fv                      vglProcs->gl.ProgramUniformMatrix2x4fv
#define vglProgramUniformMatrix3dv                        vglProcs->gl.ProgramUniformMatrix3dv
#define vglProgramUniformMatrix3fv                        vglProcs->gl.ProgramUniformMatrix3fv
#define vglProgramUniformMatrix3x2dv                      vglProcs->gl.ProgramUniformMatrix3x2dv
#define vglProgramUniformMatrix3x2fv                      vglProcs->gl.ProgramUniformMatrix3x2fv
#define vglProgramUniformMatrix3x4dv                      vglProcs->gl.ProgramUniformMatrix3x4dv
#define vglProgramUniformMatrix3x4fv                      vglProcs->gl.ProgramUniformMatrix3x4fv
#define vglProgramUniformMatrix4dv                        vglProcs->gl.ProgramUniformMatrix4dv
#define vglProgramUniformMatrix4fv                        vglProcs->gl.ProgramUniformMatrix4fv
#define vglProgramUniformMatrix4x2dv                      vglProcs->gl.ProgramUniformMatrix4x2dv
#define vglProgramUniformMatrix4x2fv                      vglProcs->gl.ProgramUniformMatrix4x2fv
#define vglProgramUniformMatrix4x3dv                      vglProcs->gl.ProgramUniformMatrix4x3dv
#define vglProgramUniformMatrix4x3fv                      vglProcs->gl.ProgramUniformMatrix4x3fv
#define vglProvokingVertex                                vglProcs->gl.ProvokingVertex
#define vglPushDebugGroup                                 vglProcs->gl.PushDebugGroup
#define vglQueryCounter                                   vglProcs->gl.QueryCounter
#define vglReadBuffer                                     vglProcs->gl.ReadBuffer
#define vglReadPixels                                     vglProcs->gl.ReadPixels
#define vglReadnPixels                                    vglProcs->gl.ReadnPixels
#define vglReleaseShaderCompiler                          vglProcs->gl.ReleaseShaderCompiler
#define vglRenderbufferStorage                            vglProcs->gl.RenderbufferStorage
#define vglRenderbufferStorageMultisample                 vglProcs->gl.RenderbufferStorageMultisample
#define vglResumeTransformFeedback                        vglProcs->gl.ResumeTransformFeedback
#define vglSampleCoverage                                 vglProcs->gl.SampleCoverage
#define vglSampleMaski                                    vglProcs->gl.SampleMaski
#define vglSamplerParameterIiv                            vglProcs->gl.SamplerParameterIiv
#define vglSamplerParameterIuiv                           vglProcs->gl.SamplerParameterIuiv
#define vglSamplerParameterf                              vglProcs->gl.SamplerParameterf
#define vglSamplerParameterfv                             vglProcs->gl.SamplerParameterfv
#define vglSamplerParameteri                              vglProcs->gl.SamplerParameteri
#define vglSamplerParameteriv                             vglProcs->gl.SamplerParameteriv
#define vglScissor                                        vglProcs->gl.Scissor
#define vglScissorArrayv                                  vglProcs->gl.ScissorArrayv
#define vglScissorIndexed                                 vglProcs->gl.ScissorIndexed
#define vglScissorIndexedv                                vglProcs->gl.ScissorIndexedv
#define vglShaderBinary                                   vglProcs->gl.ShaderBinary
#define vglShaderSource                                   vglProcs->gl.ShaderSource
#define vglShaderStorageBlockBinding                      vglProcs->gl.ShaderStorageBlockBinding
#define vglSpecializeShader                               vglProcs->gl.SpecializeShader
#define vglStencilFunc                                    vglProcs->gl.StencilFunc
#define vglStencilFuncSeparate                            vglProcs->gl.StencilFuncSeparate
#define vglStencilMask                                    vglProcs->gl.StencilMask
#define vglStencilMaskSeparate                            vglProcs->gl.StencilMaskSeparate
#define vglStencilOp                                      vglProcs->gl.StencilOp
#define vglStencilOpSeparate                              vglProcs->gl.StencilOpSeparate
#define vglTexBuffer                                      vglProcs->gl.TexBuffer
#define vglTexBufferRange                                 vglProcs->gl.TexBufferRange
#define vglTexImage1D                                     vglProcs->gl.TexImage1D
#define vglTexImage2D                                     vglProcs->gl.TexImage2D
#define vglTexImage2DMultisample                          vglProcs->gl.TexImage2DMultisample
#define vglTexImage3D                                     vglProcs->gl.TexImage3D
#define vglTexImage3DMultisample                          vglProcs->gl.TexImage3DMultisample
#define vglTexParameterIiv                                vglProcs->gl.TexParameterIiv
#define vglTexParameterIuiv                               vglProcs->gl.TexParameterIuiv
#define vglTexParameterf                                  vglProcs->gl.TexParameterf
#define vglTexParameterfv                                 vglProcs->gl.TexParameterfv
#define vglTexParameteri                                  vglProcs->gl.TexParameteri
#define vglTexParameteriv                                 vglProcs->gl.TexParameteriv
#define vglTexStorage1D                                   vglProcs->gl.TexStorage1D
#define vglTexStorage2D                                   vglProcs->gl.TexStorage2D
#define vglTexStorage2DMultisample                        vglProcs->gl.TexStorage2DMultisample
#define vglTexStorage3D                                   vglProcs->gl.TexStorage3D
#define vglTexStorage3DMultisample                        vglProcs->gl.TexStorage3DMultisample
#define vglTexSubImage1D                                  vglProcs->gl.TexSubImage1D
#define vglTexSubImage2D                                  vglProcs->gl.TexSubImage2D
#define vglTexSubImage3D                                  vglProcs->gl.TexSubImage3D
#define vglTextureBarrier                                 vglProcs->gl.TextureBarrier
#define vglTextureBuffer                                  vglProcs->gl.TextureBuffer
#define vglTextureBufferRange                             vglProcs->gl.TextureBufferRange
#define vglTextureParameterIiv                            vglProcs->gl.TextureParameterIiv
#define vglTextureParameterIuiv                           vglProcs->gl.TextureParameterIuiv
#define vglTextureParameterf                              vglProcs->gl.TextureParameterf
#define vglTextureParameterfv                             vglProcs->gl.TextureParameterfv
#define vglTextureParameteri                              vglProcs->gl.TextureParameteri
#define vglTextureParameteriv                             vglProcs->gl.TextureParameteriv
#define vglTextureStorage1D                               vglProcs->gl.TextureStorage1D
#define vglTextureStorage2D                               vglProcs->gl.TextureStorage2D
#define vglTextureStorage2DMultisample                    vglProcs->gl.TextureStorage2DMultisample
#define vglTextureStorage3D                               vglProcs->gl.TextureStorage3D
#define vglTextureStorage3DMultisample                    vglProcs->gl.TextureStorage3DMultisample
#define vglTextureSubImage1D                              vglProcs->gl.TextureSubImage1D
#define vglTextureSubImage2D                              vglProcs->gl.TextureSubImage2D
#define vglTextureSubImage3D                              vglProcs->gl.TextureSubImage3D
#define vglTextureView                                    vglProcs->gl.TextureView
#define vglTransformFeedbackBufferBase                    vglProcs->gl.TransformFeedbackBufferBase
#define vglTransformFeedbackBufferRange                   vglProcs->gl.TransformFeedbackBufferRange
#define vglTransformFeedbackVaryings                      vglProcs->gl.TransformFeedbackVaryings
#define vglUniform1d                                      vglProcs->gl.Uniform1d
#define vglUniform1dv                                     vglProcs->gl.Uniform1dv
#define vglUniform1f                                      vglProcs->gl.Uniform1f
#define vglUniform1fv                                     vglProcs->gl.Uniform1fv
#define vglUniform1i                                      vglProcs->gl.Uniform1i
#define vglUniform1iv                                     vglProcs->gl.Uniform1iv
#define vglUniform1ui                                     vglProcs->gl.Uniform1ui
#define vglUniform1uiv                                    vglProcs->gl.Uniform1uiv
#define vglUniform2d                                      vglProcs->gl.Uniform2d
#define vglUniform2dv                                     vglProcs->gl.Uniform2dv
#define vglUniform2f                                      vglProcs->gl.Uniform2f
#define vglUniform2fv                                     vglProcs->gl.Uniform2fv
#define vglUniform2i                                      vglProcs->gl.Uniform2i
#define vglUniform2iv                                     vglProcs->gl.Uniform2iv
#define vglUniform2ui                                     vglProcs->gl.Uniform2ui
#define vglUniform2uiv                                    vglProcs->gl.Uniform2uiv
#define vglUniform3d                                      vglProcs->gl.Uniform3d
#define vglUniform3dv                                     vglProcs->gl.Uniform3dv
#define vglUniform3f                                      vglProcs->gl.Uniform3f
#define vglUniform3fv                                     vglProcs->gl.Uniform3fv
#define vglUniform3i                                      vglProcs->gl.Uniform3i
#define vglUniform3iv                                     vglProcs->gl.Uniform3iv
#define vglUniform3ui                                     vglProcs->gl.Uniform3ui
#define vglUniform3uiv                                    vglProcs->gl.Uniform3uiv
#define vglUniform4d                                      vglProcs->gl.Uniform4d
#define vglUniform4dv                                     vglProcs->gl.Uniform4dv
#define vglUniform4f                                      vglProcs->gl.Uniform4f
#define vglUniform4fv                                     vglProcs->gl.Uniform4fv
#define vglUniform4i                                      vglProcs->gl.Uniform4i
#define vglUniform4iv                                     vglProcs->gl.Uniform4iv
#define vglUniform4ui                                     vglProcs->gl.Uniform4ui
#define vglUniform4uiv                                    vglProcs->gl.Uniform4uiv
#define vglUniformBlockBinding                            vglProcs->gl.UniformBlockBinding
#define vglUniformMatrix2dv                               vglProcs->gl.UniformMatrix2dv
#define vglUniformMatrix2fv                               vglProcs->gl.UniformMatrix2fv
#define vglUniformMatrix2x3dv                             vglProcs->gl.UniformMatrix2x3dv
#define vglUniformMatrix2x3fv                             vglProcs->gl.UniformMatrix2x3fv
#define vglUniformMatrix2x4dv                             vglProcs->gl.UniformMatrix2x4dv
#define vglUniformMatrix2x4fv                             vglProcs->gl.UniformMatrix2x4fv
#define vglUniformMatrix3dv                               vglProcs->gl.UniformMatrix3dv
#define vglUniformMatrix3fv                               vglProcs->gl.UniformMatrix3fv
#define vglUniformMatrix3x2dv                             vglProcs->gl.UniformMatrix3x2dv
#define vglUniformMatrix3x2fv                             vglProcs->gl.UniformMatrix3x2fv
#define vglUniformMatrix3x4dv                             vglProcs->gl.UniformMatrix3x4dv
#define vglUniformMatrix3x4fv                             vglProcs->gl.UniformMatrix3x4fv
#define vglUniformMatrix4dv                               vglProcs->gl.UniformMatrix4dv
#define vglUniformMatrix4fv                               vglProcs->gl.UniformMatrix4fv
#define vglUniformMatrix4x2dv                             vglProcs->gl.UniformMatrix4x2dv
#define vglUniformMatrix4x2fv                             vglProcs->gl.UniformMatrix4x2fv
#define vglUniformMatrix4x3dv                             vglProcs->gl.UniformMatrix4x3dv
#define vglUniformMatrix4x3fv                             vglProcs->gl.UniformMatrix4x3fv
#define vglUniformSubroutinesuiv                          vglProcs->gl.UniformSubroutinesuiv
#define vglUnmapBuffer                                    vglProcs->gl.UnmapBuffer
#define vglUnmapNamedBuffer                               vglProcs->gl.UnmapNamedBuffer
#define vglUseProgram                                     vglProcs->gl.UseProgram
#define vglUseProgramStages                               vglProcs->gl.UseProgramStages
#define vglValidateProgram                                vglProcs->gl.ValidateProgram
#define vglValidateProgramPipeline                        vglProcs->gl.ValidateProgramPipeline
#define vglVertexArrayAttribBinding                       vglProcs->gl.VertexArrayAttribBinding
#define vglVertexArrayAttribFormat                        vglProcs->gl.VertexArrayAttribFormat
#define vglVertexArrayAttribIFormat                       vglProcs->gl.VertexArrayAttribIFormat
#define vglVertexArrayAttribLFormat                       vglProcs->gl.VertexArrayAttribLFormat
#define vglVertexArrayBindingDivisor                      vglProcs->gl.VertexArrayBindingDivisor
#define vglVertexArrayElementBuffer                       vglProcs->gl.VertexArrayElementBuffer
#define vglVertexArrayVertexBuffer                        vglProcs->gl.VertexArrayVertexBuffer
#define vglVertexArrayVertexBuffers                       vglProcs->gl.VertexArrayVertexBuffers
#define vglVertexAttrib1d                                 vglProcs->gl.VertexAttrib1d
#define vglVertexAttrib1dv                                vglProcs->gl.VertexAttrib1dv
#define vglVertexAttrib1f                                 vglProcs->gl.VertexAttrib1f
#define vglVertexAttrib1fv                                vglProcs->gl.VertexAttrib1fv
#define vglVertexAttrib1s                                 vglProcs->gl.VertexAttrib1s
#define vglVertexAttrib1sv                                vglProcs->gl.VertexAttrib1sv
#define vglVertexAttrib2d                                 vglProcs->gl.VertexAttrib2d
#define vglVertexAttrib2dv                                vglProcs->gl.VertexAttrib2dv
#define vglVertexAttrib2f                                 vglProcs->gl.VertexAttrib2f
#define vglVertexAttrib2fv                                vglProcs->gl.VertexAttrib2fv
#define vglVertexAttrib2s                                 vglProcs->gl.VertexAttrib2s
#define vglVertexAttrib2sv                                vglProcs->gl.VertexAttrib2sv
#define vglVertexAttrib3d                                 vglProcs->gl.VertexAttrib3d
#define vglVertexAttrib3dv                                vglProcs->gl.VertexAttrib3dv
#define vglVertexAttrib3f                                 vglProcs->gl.VertexAttrib3f
#define vglVertexAttrib3fv                                vglProcs->gl.VertexAttrib3fv
#define vglVertexAttrib3s                                 vglProcs->gl.VertexAttrib3s
#define vglVertexAttrib3sv                                vglProcs->gl.VertexAttrib3sv
#define vglVertexAttrib4Nbv                               vglProcs->gl.VertexAttrib4Nbv
#define vglVertexAttrib4Niv                               vglProcs->gl.VertexAttrib4Niv
#define vglVertexAttrib4Nsv                               vglProcs->gl.VertexAttrib4Nsv
#define vglVertexAttrib4Nub                               vglProcs->gl.VertexAttrib4Nub
#define vglVertexAttrib4Nubv                              vglProcs->gl.VertexAttrib4Nubv
#define vglVertexAttrib4Nuiv                              vglProcs->gl.VertexAttrib4Nuiv
#define vglVertexAttrib4Nusv                              vglProcs->gl.VertexAttrib4Nusv
#define vglVertexAttrib4bv                                vglProcs->gl.VertexAttrib4bv
#define vglVertexAttrib4d                                 vglProcs->gl.VertexAttrib4d
#define vglVertexAttrib4dv                                vglProcs->gl.VertexAttrib4dv
#define vglVertexAttrib4f                                 vglProcs->gl.VertexAttrib4f
#define vglVertexAttrib4fv                                vglProcs->gl.VertexAttrib4fv
#define vglVertexAttrib4iv                                vglProcs->gl.VertexAttrib4iv
#define vglVertexAttrib4s                                 vglProcs->gl.VertexAttrib4s
#define vglVertexAttrib4sv                                vglProcs->gl.VertexAttrib4sv
#define vglVertexAttrib4ubv                               vglProcs->gl.VertexAttrib4ubv
#define vglVertexAttrib4uiv                               vglProcs->gl.VertexAttrib4uiv
#define vglVertexAttrib4usv                               vglProcs->gl.VertexAttrib4usv
#define vglVertexAttribBinding                            vglProcs->gl.VertexAttribBinding
#define vglVertexAttribDivisor                            vglProcs->gl.VertexAttribDivisor
#define vglVertexAttribFormat                             vglProcs->gl.VertexAttribFormat
#define vglVertexAttribI1i                                vglProcs->gl.VertexAttribI1i
#define vglVertexAttribI1iv                               vglProcs->gl.VertexAttribI1iv
#define vglVertexAttribI1ui                               vglProcs->gl.VertexAttribI1ui
#define vglVertexAttribI1uiv                              vglProcs->gl.VertexAttribI1uiv
#define vglVertexAttribI2i                                vglProcs->gl.VertexAttribI2i
#define vglVertexAttribI2iv                               vglProcs->gl.VertexAttribI2iv
#define vglVertexAttribI2ui                               vglProcs->gl.VertexAttribI2ui
#define vglVertexAttribI2uiv                              vglProcs->gl.VertexAttribI2uiv
#define vglVertexAttribI3i                                vglProcs->gl.VertexAttribI3i
#define vglVertexAttribI3iv                               vglProcs->gl.VertexAttribI3iv
#define vglVertexAttribI3ui                               vglProcs->gl.VertexAttribI3ui
#define vglVertexAttribI3uiv                              vglProcs->gl.VertexAttribI3uiv
#define vglVertexAttribI4bv                               vglProcs->gl.VertexAttribI4bv
#define vglVertexAttribI4i                                vglProcs->gl.VertexAttribI4i
#define vglVertexAttribI4iv                               vglProcs->gl.VertexAttribI4iv
#define vglVertexAttribI4sv                               vglProcs->gl.VertexAttribI4sv
#define vglVertexAttribI4ubv                              vglProcs->gl.VertexAttribI4ubv
#define vglVertexAttribI4ui                               vglProcs->gl.VertexAttribI4ui
#define vglVertexAttribI4uiv                              vglProcs->gl.VertexAttribI4uiv
#define vglVertexAttribI4usv                              vglProcs->gl.VertexAttribI4usv
#define vglVertexAttribIFormat                            vglProcs->gl.VertexAttribIFormat
#define vglVertexAttribIPointer                           vglProcs->gl.VertexAttribIPointer
#define vglVertexAttribL1d                                vglProcs->gl.VertexAttribL1d
#define vglVertexAttribL1dv                               vglProcs->gl.VertexAttribL1dv
#define vglVertexAttribL2d                                vglProcs->gl.VertexAttribL2d
#define vglVertexAttribL2dv                               vglProcs->gl.VertexAttribL2dv
#define vglVertexAttribL3d                                vglProcs->gl.VertexAttribL3d
#define vglVertexAttribL3dv                               vglProcs->gl.VertexAttribL3dv
#define vglVertexAttribL4d                                vglProcs->gl.VertexAttribL4d
#define vglVertexAttribL4dv                               vglProcs->gl.VertexAttribL4dv
#define vglVertexAttribLFormat                            vglProcs->gl.VertexAttribLFormat
#define vglVertexAttribLPointer                           vglProcs->gl.VertexAttribLPointer
#define vglVertexAttribP1ui                               vglProcs->gl.VertexAttribP1ui
#define vglVertexAttribP1uiv                              vglProcs->gl.VertexAttribP1uiv
#define vglVertexAttribP2ui                               vglProcs->gl.VertexAttribP2ui
#define vglVertexAttribP2uiv                              vglProcs->gl.VertexAttribP2uiv
#define vglVertexAttribP3ui                               vglProcs->gl.VertexAttribP3ui
#define vglVertexAttribP3uiv                              vglProcs->gl.VertexAttribP3uiv
#define vglVertexAttribP4ui                               vglProcs->gl.VertexAttribP4ui
#define vglVertexAttribP4uiv                              vglProcs->gl.VertexAttribP4uiv
#define vglVertexAttribPointer                            vglProcs->gl.VertexAttribPointer
#define vglVertexBindingDivisor                           vglProcs->gl.VertexBindingDivisor
#define vglViewport                                       vglProcs->gl.Viewport
#define vglViewportArrayv                                 vglProcs->gl.ViewportArrayv
#define vglViewportIndexedf                               vglProcs->gl.ViewportIndexedf
#define vglViewportIndexedfv                              vglProcs->gl.ViewportIndexedfv
#define vglWaitSync                                       vglProcs->gl.WaitSync

#ifdef __cplusplus
}
#endif

#endif
