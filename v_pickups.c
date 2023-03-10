typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef struct {
    float x;
    float y;
    float z;

} CVector;

int (*printf)(const char *, ...) = (int (*)(const char *, ...))0x54F7D0;
void (*PrintString)(float, float, const char*) = (void (*)(float, float, const char*))0x2A6E20;
void (*updatePickups)(void) = (void (*)(void))0x2D9390;
CVector * (*FindPlayerCoors)(CVector *outPoint, int playerIndex) = (CVector * (*)(CVector *, int))0x263C90;
uint32_t** ms_modelInfoPtrs = (uint32_t**)0x7A8780;

uint32_t* TheCamera = (uint32_t*)0x6FE530;

void (*GetBoundCentre)(uint32_t, CVector*) = (void (*)(uint32_t, CVector*))0x22F3B0;
float (*FindGroundZFor3DCoord)(float x, float y, float z, int *b, uint32_t **ent) = (float (*)(float, float, float, int *, uint32_t **))0x2869C0;
void (*ClearSpaceForMissionEntity)(CVector* a1, uint32_t object) = (void (*)(CVector*, uint32_t))0x30AA10;

void (*CWorld_Remove)(uint32_t) = (void (*)(uint32_t))0x27DF60;
void (*CEntity_UpdateRwFrame)(uint32_t) = (void (*)(uint32_t))0x22F470;
void (*CWorld_Add)(uint32_t) = (void (*)(uint32_t))0x27DEA0;
void (*CSimpleTransform_UpdateRwMatrix)(int a1, int a2) = (void (*)(int, int))0x25FA20;
void (*CMatrix_UpdateRwMatrix)(uint32_t *a1, uint32_t *a2) = (void (*)(uint32_t *, uint32_t *))0x10ED80;

void (*CEntity_SetOrientation)(uint32_t, float, float, float) = (void (*)(uint32_t, float, float, float))0x10F200;

float VectorSqrMagnitude(CVector vector) { return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z; }

void writeMemory(uint32_t* Address, uint32_t size, uint32_t value)
{
	uint32_t align_diff;
	uint32_t aligned_addr;
	uint32_t temp[2];
	uint8_t uint8_ts[8];

	switch(size){
	default:
		align_diff = (uint32_t)Address % 4;

		if(align_diff > 0){
			aligned_addr = (uint32_t)Address;
			aligned_addr = ((uint32_t)aligned_addr - align_diff);
			temp[0] = *(uint32_t*)(aligned_addr);

			uint8_ts[0] = temp[0] & 0xFF; uint8_ts[1] = (temp[0] >> 8) & 0xFF; uint8_ts[2] = (temp[0] >> 16) & 0xFF; uint8_ts[3] = (temp[0] >> 24) & 0xFF;

			uint8_ts[align_diff] = (uint8_t)value;

			*(uint32_t*)aligned_addr =  uint8_ts[0] | (uint8_ts[1] << 8) | (uint8_ts[2] << 16) | (uint8_ts[3] << 24);
		} else {
			*Address = (uint8_t)value;
		}

		break;

	case 2:
		align_diff = (uint32_t)Address % 4;

		if(align_diff > 0){
			aligned_addr = (uint32_t)Address;
			aligned_addr = ((uint32_t)aligned_addr - align_diff);
			temp[0] = *(uint32_t*)(aligned_addr);
			temp[1] = *(uint32_t*)(aligned_addr+4);

			uint8_ts[0] = temp[0] & 0xFF; 
            uint8_ts[1] = (temp[0] >> 8) & 0xFF; 
            uint8_ts[2] = (temp[0] >> 16) & 0xFF; 
            uint8_ts[3] = (temp[0] >> 24) & 0xFF;

			if(align_diff > 2) {
                uint8_ts[4] = temp[1] & 0xFF; 
                uint8_ts[5] = (temp[1] >> 8) & 0xFF; 
                uint8_ts[6] = (temp[2] >> 16) & 0xFF; 
                uint8_ts[7] = (temp[1] >> 24) & 0xFF;
            }

			uint8_ts[align_diff] = (uint16_t)value & 0xFF; 
            uint8_ts[align_diff+1] = ((uint16_t)value >> 8) & 0xFF;

			*(uint32_t*)aligned_addr =  uint8_ts[0] | (uint8_ts[1] << 8) | (uint8_ts[2] << 16) | (uint8_ts[3] << 24);
			if(align_diff > 2) *(uint32_t*)(aligned_addr+4) =  uint8_ts[4] | (uint8_ts[5] << 8) | (uint8_ts[6] << 16) | (uint8_ts[7] << 24);

		} else {
			*Address = (uint16_t)value;
		}
		
		break;

	case 4:
		*Address = value;
		break;
	}
}

uint32_t readMemory(uint32_t* Address, int size)
{
	uint32_t align_diff;
	uint32_t aligned_addr;
    uint32_t result;

	switch(size){
	case 1:
		align_diff = (uint32_t)Address % 4;
		aligned_addr = (uint32_t)Address;

		if(align_diff > 0){
			aligned_addr = ((uint32_t)aligned_addr - align_diff);
			result = (uint8_t)(*(uint32_t*)(aligned_addr) >> (8*align_diff));
		} else {
			result = *(uint8_t*)Address;
		}
		break;
	case 2:
		align_diff = (uint32_t)Address % 4;
		aligned_addr = (uint32_t)Address;

		if(align_diff > 0){
			aligned_addr = ((uint32_t)aligned_addr - align_diff);
			result = (uint16_t)(*(uint32_t*)(aligned_addr) >> (8*align_diff));
			result += (uint16_t)(*(uint32_t*)(aligned_addr+4) << (8*(4-align_diff)));
		} else {
			result = *(uint16_t*)Address;
		}

		break;
	case 4:
		result = *Address;
		break;
	}

	return result;
}

void RedirectCall(uint32_t* call_addr, uint32_t addr){
    *call_addr = 0x0C000000+(addr/4);
}

void setObjectRotation(uint32_t object, float x, float y, float z) {
    int v16, v17, v18, v19;
    float v11, v12, v13, v15, v94, v95, v96;
    float *v10, *v14;

    CWorld_Remove(object);

    v10 = *(float **)(object + 20);
    v11 = (float)(3.1415927f * z) / 180.0f;
    if ( v10 )
    {
        v12 = v10[13];
        v13 = v10[14];
        v94 = v10[12];
        v95 = v12;
        v96 = v13;
        CEntity_SetOrientation(*(uint32_t *)(object + 20), (float)(3.1415927f * x) / 180.0f, (float)(3.1415927f * y) / 180.0f, v11);
        v14 = *(float **)(object + 20);
        v15 = v96;
        v16 = v95;
        v14[12] = v14[12] + v94;
        v14[13] = v14[13] + v16;
        v14[14] = v14[14] + v15;
    } else {
        *(float *)(object + 16) = v11;
    }

    v17 = *(uint32_t *)(object + 24);
    if ( v17 )
    {
        v18 = *(uint32_t **)(object + 20);
        v19 = *(uint32_t *)(v17 + 4) + 16;
        if ( v18 )
            CMatrix_UpdateRwMatrix(v18, v19);
        else
            CSimpleTransform_UpdateRwMatrix(object + 4, v19);
    }

    CEntity_UpdateRwFrame(object);
    CWorld_Add(object);
}

void pickups_on_ground() {
    uint32_t pickup, pickup_type, x, pObject, wModelId, v17;
    CVector* coords, player_coords, tmp;
    float min_radius = 45.0f;
    float ground_z;
    char b;

    updatePickups();

    for(pickup = 0x803170; pickup <= 0x807ED0; pickup += 0x20) {
        pickup_type = readMemory(pickup+0x1C, 1);

        if (pickup_type == 2 || pickup_type == 3 || pickup_type == 4 || pickup_type == 5 ||
            pickup_type == 8 || pickup_type == 15 || pickup_type == 19 ) {

            x = readMemory(pickup+0x10, 2);
            pObject = readMemory(pickup+0x4, 4);
            wModelId = readMemory(pickup+0x18, 2);

            if (x != 0 && pObject != 0) {

                v17 = *(uint32_t *)(pObject + 20);
                if ( v17 )
                    coords = (CVector *)(v17 + 48);
                else
                    coords = (CVector *)(pObject + 4);

                FindPlayerCoors(&player_coords, -1);

                tmp.x = player_coords.x - coords->x;
                tmp.y = player_coords.y - coords->y;
                tmp.z = player_coords.z - coords->z;

                if ((VectorSqrMagnitude(tmp) <= min_radius * min_radius)) {
                    ground_z = FindGroundZFor3DCoord(coords->x, coords->y, coords->z, &b, 0);
                    ground_z += 0.05f;
                    
                    coords->z = ground_z;
                    (*(void (**)(int, CVector *, uint32_t))(*(uint32_t *)pObject + 64))(pObject, coords, 0);

                    ClearSpaceForMissionEntity(coords, pObject);
                    
                    if (wModelId != 1212) {
                        setObjectRotation(pObject, 90.0f, 0.0f, 134.0f);
                    } else {
                        setObjectRotation(pObject, 0.0f, 0.0f, 5.0f);
                    }
                }

            }

           }

    }
}

int _start()
{  
    writeMemory(0x2D9E10,4,0x0); //object->__parent.physical.entity.placeable.m_pCoords->pos.z = outVec.z;
    writeMemory(0x22FD90,4,0x2403FFFF);   // movzx edx,word ptr [008CD59C]

    writeMemory(0x2DD3B0,4,0x100000B7);
    writeMemory(0x2DD3B4,4,0x0); 
    
    writeMemory(0x2DCA10,4,0x0); //coronas

    RedirectCall(0x2435E0, pickups_on_ground);
    return 0;
}
