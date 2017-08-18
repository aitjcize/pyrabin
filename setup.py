from distutils.core import setup, Extension

setup(
    name='PyRabin',
    version='0.6',
    description = 'A python for generating rabin fingerprints.',
    author = 'Wei-Ning Huang (AZ)',
    author_email = 'aitjcize@gmail.com',
    url = 'http://github.com/aitjcize/pyrabin',
    license = 'GPL',
    ext_modules=[
        Extension('rabin', [
                'src/pyrabin.c',
                'src/rabin.c',
                'src/rabin_polynomial.c',
                'src/utils.c',
       ], libraries=['crypto'])
    ],
    classifiers = [
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
	'Programming Language :: Python :: 3.3',
	'Programming Language :: Python :: 3.4',
	'Programming Language :: Python :: 3.5',
	'Programming Language :: Python :: 3.6',
	'Programming Language :: Python :: 3.7',
    ],
)
