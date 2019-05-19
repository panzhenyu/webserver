#ifndef _MIME_H
#define _MIME_H

enum MIME
{
	JPG, PNG, GIF, TIF, BMP,
	DWG, PSD, RTF, XML, HTML,
	EML, DBX, PST, XL, MDB,
	WPD, EPS, PDF, QDF, PWL,
	ZIP, RAR, WAV, AVI, RAM,
	RM, MPG, MOV, ASF, MID
};

/* 根据MIME类型t返回其对应的字符串，如果找不到相应的MIME类型则返回NULL */
char* mime2str(enum MIME t);

// struct MIME
// {
// 	char name[10];
// 	char data[12];
// }fn={{jpg,FFD8FF},{png,89504E47},{gif,47494638},{tif,49492A00},{bmp,24D},{dwg,1433130},{psd,8425053},{rtf,7B5C727466},{xml,3C3F786D6C},
// {html,68746D6C3E},{eml,44656C6976},{dbx,CFAD12FEC5},{pst,2142444E},{xl,D0CF11E0},{mdb,5374616E64},{wpd,FF575043},{eps,252150532D},
// {pdf,255044462D},{qdf,AC9EBD8F},{pwl,E3828596},{zip,504B0304},{rar,52617221},{wav,57415645},{avi,41564920},{ram,2E7261FD},{rm,2E524D46},
// {mpg,000001BA},{mpg,000001B3},{mov,6D6F6F76},{asf,3026B2758E6},{mid,4D546864}}

#endif
