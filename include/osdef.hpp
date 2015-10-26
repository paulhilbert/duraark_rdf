#ifndef _DURAARK_RDF_OSDEF_HPP_
#define _DURAARK_RDF_OSDEF_HPP_

#ifdef _WIN32
#define EXPORT_DECL __declspec(dllexport)
#else
#define EXPORT_DECL
#endif

#endif /* _DURAARK_RDF_OSDEF_HPP_ */
