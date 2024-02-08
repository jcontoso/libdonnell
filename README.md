# libdonnell
libdonnell is a C library for drawing Unicode text and simple GUIs onto image buffers, while it was designed specifically for use with the Wayland windowing system, it can be used with basically anything due to its generic nature.

It currently boasts the following features: 
- Only depends on libpng, FontConfig, FreeType, GNU FriBidi and GNU Unistring,
- Supports a vast subset of Unicode text by default (including Arabic text).
- Can load in HarfBuzz dynamically for even greater Unicode coverage if the system has it installed.
- Can load in an external SVG renderer for Emoji fonts that use SVG glyphs (Currently, only librsvg and NanoSVG built with position independent code are supported).
- Supports some basic GUI rendering features, like buttons and icons.
- Supports integer scaling for all GUI rendering operations.

# Notes for users
- You can force disable librsvg support by setting the LIBDONNELL_DISABLE_RSVG environment variable to 1.
- You can force disable HarfBuzz support by setting the LIBDONNELL_DISABLE_HB environment variable to 1.
