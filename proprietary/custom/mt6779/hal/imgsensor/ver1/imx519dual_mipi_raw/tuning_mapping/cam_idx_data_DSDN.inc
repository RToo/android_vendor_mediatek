#define IDX_DATA_DSDN_DIM_NS    5
#define IDX_DATA_DSDN_FACTOR_SZ    3
#define IDX_DATA_DSDN_ENTRY_NS    8

static unsigned int _cam_data_entry_DSDN_key0000[] = {0X000001FC, 0X80700000, 0X00005900, };
static unsigned int _cam_data_entry_DSDN_key0001[] = {0X000001FC, 0X40700000, 0X00005900, };
static unsigned int _cam_data_entry_DSDN_key0002[] = {0X000001FC, 0X80700000, 0X00006900, };
static unsigned int _cam_data_entry_DSDN_key0003[] = {0X000001FC, 0X40700000, 0X00006900, };
static unsigned int _cam_data_entry_DSDN_key0004[] = {0XFC000000, 0XC000000F, 0X0003F1FF, };
static unsigned int _cam_data_entry_DSDN_key0005[] = {0X0001E1FC, 0XC0700000, 0X0003F6FF, };
static unsigned int _cam_data_entry_DSDN_key0006[] = {0X000001FC, 0X80700000, 0X0001BF00, };
static unsigned int _cam_data_entry_DSDN_key0007[] = {0XFFFFFFFF, 0XBFFFFFFF, 0X0003FF00, };

static IDX_MASK_ENTRY _cam_data_entry_DSDN[IDX_DATA_DSDN_ENTRY_NS] =
{
    {(unsigned int*)&_cam_data_entry_DSDN_key0000, 0, 0, 0, 0},
    {(unsigned int*)&_cam_data_entry_DSDN_key0001, 0, 1, 0, 0},
    {(unsigned int*)&_cam_data_entry_DSDN_key0002, 1, 2, 0, 0},
    {(unsigned int*)&_cam_data_entry_DSDN_key0003, 1, 3, 0, 0},
    {(unsigned int*)&_cam_data_entry_DSDN_key0004, 0, 14, 0, 0},
    {(unsigned int*)&_cam_data_entry_DSDN_key0005, 0, 18, 0, 0},
    {(unsigned int*)&_cam_data_entry_DSDN_key0006, 0, 22, 0, 0},
    {(unsigned int*)&_cam_data_entry_DSDN_key0007, 0, 24, 0, 0},
};

static unsigned short _cam_data_dims_DSDN[] = 
{
    EDim_IspProfile,
    EDim_SensorMode,
    EDim_Flash,
    EDim_FaceDetection,
    EDim_Zoom,
};

static unsigned short _cam_data_expand_DSDN[] = 
{0, 0, 0};

const IDX_MASK_T cam_data_DSDN =
{
    {IDX_ALGO_MASK, IDX_DATA_DSDN_DIM_NS, (unsigned short*)&_cam_data_dims_DSDN, (unsigned short*)&_cam_data_expand_DSDN},
    {IDX_DATA_DSDN_ENTRY_NS, IDX_DATA_DSDN_FACTOR_SZ, (IDX_MASK_ENTRY*)&_cam_data_entry_DSDN}
};