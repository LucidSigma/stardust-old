#include <cstdlib>

#include "sandbox/TestScene.h"
#include "stardust/Stardust.h"

int main([[maybe_unused]] const int argc, char* argv[])
{
	const stardust::Application::FilepathsInfo gameFilepaths{
		.windowIcon = "assets/icon/icon.png",
		.config = "config/config.json",
		.logFile = "log.txt",
		.screenshots = "screenshots",
	};

	const stardust::Application::FilesystemCreateInfo filesystemInfo{
		.argv0 = argv[0],
		.assetsArchive = "assets.pak",
		.localesArchive = "locales.pak",
	};

	stardust::Application game({
		.title = "Stardust",
		.filepaths = gameFilepaths,
		.filesystem = filesystemInfo,

		.fixedTimestep = 0.01f,

		.requireRenderToTexture = true,
		.rendererLogicalSize = glm::uvec2{ 1920u, 1080u },
	});

	if (!game.DidInitialiseSuccessfully())
	{
		return EXIT_FAILURE;
	}

	game.GetSceneManager().PushScene<TestScene>(game, "Test");
	game.Run();

	return EXIT_SUCCESS;
}