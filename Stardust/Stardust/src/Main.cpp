#include <cstdlib>

#include "sandbox/TestScene.h"
#include "stardust/Stardust.h"

int main([[maybe_unused]] const int argc, char* argv[])
{
	/*std::unique_ptr<SoLoud::Soloud> soundSystem = std::make_unique<SoLoud::Soloud>();
	soundSystem->init();

	SoLoud::WavStream sample;
	std::cout << sample.load("test.wav") << "\n";

	auto handle = soundSystem->play(sample, 1.0f, 0.0f, true);
	soundSystem->setVolume(handle, 0.2f);

	std::cout << std::boolalpha << (soundSystem->isValidVoiceHandle(handle) && !soundSystem->getPause(handle)) << "\n";
	soundSystem->setPause(handle, false);
	std::cout << std::boolalpha << (soundSystem->isValidVoiceHandle(handle) && !soundSystem->getPause(handle)) << "\n";

	std::this_thread::sleep_for(std::chrono::seconds(2));
	soundSystem->oscillateGlobalVolume(0.5f, 1.0f, 2.0f);
	std::this_thread::sleep_for(std::chrono::seconds(10));
	soundSystem->oscillateGlobalVolume(1.0f, 1.0f, 0.0f);

	while (soundSystem->isValidVoiceHandle(handle))
	{
		;
	}

	soundSystem->deinit();*/

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