#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "smaz.h"

int main(void) {
    char in[512];
    char out[4096];
    char d[4096];
    int comprlen, decomprlen;
    int j, ranlen;
    int times = 1000000;
    float result_compression;
    char *strings[] = {
        "Tentar não. Faça ou não faça. Tentativa não há. ",
        "É uma armadilha! ",
        "O medo é o caminho para o lado negro. ",
        "Um mago nunca se atrasa Frodo Bolseiro, nem se adianta, ele chega exatamente quando pretende chegar. ",
        "Tudo o que temos de decidir é o que fazer com o tempo que nos é dado. ",
        NULL
    };

    result_compression=0.0;
    j=0;
    while(strings[j]) {
        int comprlevel;

        comprlen = smaz_compress(strings[j],strlen(strings[j]),out,sizeof(out));
        comprlevel = 100-((100*comprlen)/strlen(strings[j]));
        decomprlen = smaz_decompress(out,comprlen,d,sizeof(d));
        if (strlen(strings[j]) != (unsigned)decomprlen ||
            memcmp(strings[j],d,decomprlen))
        {
            printf("BUG: error compressing '%s'\n", strings[j]);
            exit(1);
        }
        if (comprlevel < 0) {
            printf("'%s' enlarged by %d%%\n",strings[j],-comprlevel);
        } else {
            printf("'%s' compressed by %d%%\n",strings[j],comprlevel);
        }
        j++;
        result_compression = result_compression + comprlevel;
    }

    printf("Average compression result: %.2f%%\n", result_compression / j);
    printf("Encrypting and decrypting %d test strings...\n", times);
    while(times--) {
        char charset[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvxyz/. ";
        ranlen = random() % 512;

        for (j = 0; j < ranlen; j++) {
            if (times & 1)
                in[j] = charset[random() % (sizeof(charset)-1)];
            else
                in[j] = (char)(random() & 0xff);
        }
        comprlen = smaz_compress(in,ranlen,out,sizeof(out));
        decomprlen = smaz_decompress(out,comprlen,d,sizeof(out));

        if (ranlen != decomprlen || memcmp(in,d,ranlen)) {
            printf("Bug! TEST NOT PASSED\n");
            exit(1);
        }
        /* printf("%d -> %d\n", comprlen, decomprlen); */
    }
    printf("TEST PASSED :)\n");
    return 0;
}
