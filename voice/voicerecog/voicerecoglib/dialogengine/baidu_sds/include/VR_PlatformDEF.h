#ifndef VR_PLATFORMDEF_H
#define VR_PLATFORMDEF_H

enum {
    VR_PLATFORM_ERR_OK = 0,		// successful
    VR_PLATFORM_ERR_UNKNOWN,	// unknown error

    VR_PLATFORM_ERR_NOMEM,		// no memory
    VR_PLATFORM_ERR_PARAM,		// param error

    VR_PLATFORM_ERR_CONFIG,     // config file error
    VR_PLATFORM_ERR_NETWORK,	// network error when request from net
    VR_PLATFORM_ERR_ENCODE,     // encode error
    VR_PLATFORM_ERR_VR_SILENCE,	// no speech
    VR_PLATFORM_ERR_VR_TIMEOUT,	// vr time out
    VR_PLATFORM_ERR_VR_NOMATCH, // no match result
    VR_PLATFORM_ERR_VR_NOAUDIO_DEVICE, //
    VR_PLATFORM_ERR_EVENT,      // event error
    VR_PLATFORM_ERR_CONTACT_UPDATING,    // contact error
    VR_PLATFORM_ERR_TAKE_PICTURE // take picture error
};

enum VR_MAP_SCREEN_SIZE {
    VR_MAP_SCREEN_HALF=0x80000000,
    VR_MAP_SCREEN_FULL,
    VR_MAP_SCREEN_2X,
};

enum VR_MAP_ZOOM_DIR {
    VR_MAP_ZOOM_DIR_IN = 0,
    VR_MAP_ZOOM_DIR_OUT,
};

// TBD
enum VR_MAP_ZOOM {
    VR_MAP_ZOOM_IN =0,
    VR_MAP_ZOOM_OUT,
    VR_MAP_ZOOM_LEVEL_ONE,
    VR_MAP_ZOOM_LEVEL_SECOND,
    VR_MAP_ZOOM_LEVEL_THIRD,
    VR_MAP_ZOOM_LEVEL_FOURTH,
    VR_MAP_ZOOM_LEVEL_MAX,
};

enum VR_MAP_DIR {
    VR_MAP_DIR_NONE =0,
    VR_MAP_DIR_NORTH_UP,
    VR_MAP_DIR_HEADING_UP,
    VR_MAP_DIR_UP,
    VR_MAP_DIR_DOWN,
    VR_MAP_DIR_LEFT,
    VR_MAP_DIR_RIGHT,
};

enum VR_MAP_VIEW {
    VR_MAP_VIEW_IR = 0,
    VR_MAP_VIEW_SKY,
    VR_MAP_VIEW_NORMAL,
    VR_MAP_VIEW_NEXT,
};

enum VR_VOLUME_TYPE {
    VR_VOLUME_TYPE_DEFAULT = 0,
    VR_VOLUME_TYPE_SYSTEM,
    VR_VOLUME_TYPE_MUSIC,
    VR_VOLUME_TYPE_NAVI,
};

enum VR_VOLUME_OP {
    VR_VOLUME_OP_UP = 0,
    VR_VOLUME_OP_DOWN,
    VR_VOLUME_OP_ON,
    VR_VOLUME_OP_OFF,
};

enum VR_TELEPHONE_TYPE {
    VR_TEL_ANY = 0,
    VR_TEL_MOBILE,
    VR_TEL_HOME,
    VR_TEL_CORP,
};

#endif // VR_PLATFORMDEF_H