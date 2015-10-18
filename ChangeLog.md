Changes and Additions for pxCore 1.3 July 19th 2009

  * Added onMouseLeave event to pxWindow.  This event allows reliable prelighting or hover effects to be used within a window.
  * Added the Hover Sample that demonstrates using the onMouseLeave event
  * Added an initNative method for MS Windows that allows one to specify the platform specific window styles during windows creation.
  * Added a Sprite example that demonstrates blitting of a small sprite offscreen into a larger offscreen buffer.
  * Bug fixes

Changes and Additions for pxCore 1.2 February 16th 2008

  * Added the ability to directly blit pxBuffer frame buffer descriptors.
  * Added beginNativeDrawing and endNativeDrawing to allow native drawing outside of the paint loop.
  * Added an example that demonstrates using platform native drawing methods in conjunction with the portable frame buffer primitives in pxCore.