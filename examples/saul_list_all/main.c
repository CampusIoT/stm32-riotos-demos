#include <stdio.h>
#include <stdlib.h>

#include "phydat.h"
#include "saul_reg.h"
#include "saul.h"

int main(void)
{
	phydat_t res;

	puts("SAUL test application");

	while (1) {
		saul_reg_t *dev = saul_reg;

		if (dev == NULL) {
			puts("No SAUL devices present");
			return 1;
		}

		while (dev) {
			int dim = saul_reg_read(dev, &res);
			printf("\nDev: %s\tType: %s\n", dev->name,
					saul_class_to_str(dev->driver->type));
			phydat_dump(&res, dim);
			dev = dev->next;
		}

		printf("\f");
	}

	return 0;
}
