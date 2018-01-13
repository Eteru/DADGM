#pragma once


size_t GetID()
{
	static size_t crtID = 0;

	return crtID++;
}